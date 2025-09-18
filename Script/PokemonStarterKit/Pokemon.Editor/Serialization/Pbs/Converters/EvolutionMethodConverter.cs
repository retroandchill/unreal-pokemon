using System.Collections.Immutable;
using System.Numerics;
using System.Reflection;
using GameDataAccessTools.Core;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Editor.Serialization.Model.Pbs;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace Pokemon.Editor.Serialization.Pbs.Converters;

public class EvolutionMethodConverter : IPbsConverter<EvolutionConditionInfo>
{
    public Type Type => typeof(EvolutionConditionInfo);

    string IPbsConverter.WriteCsvValue(
        object? value,
        PbsScalarDescriptor schema,
        string? sectionName
    )
    {
        return WriteCsvValue((EvolutionConditionInfo)value!, schema, sectionName);
    }

    public string WriteCsvValue(
        EvolutionConditionInfo value,
        PbsScalarDescriptor schema,
        string? sectionName
    )
    {
        return ExportEvolutionParameters(value);
    }

    private static string ExportEvolutionParameters(EvolutionConditionInfo value)
    {
        if (value.Parameters.ScriptStruct is null)
        {
            return $"{value.Species},{value.Method}";
        }

        var method = GameData.Evolutions.GetEntry(value.Method);

        if (method.Parameter?.ManagedType is null)
        {
            throw new InvalidOperationException("Invalid evolution method!");
        }

        var dataParameters = method
            .Parameter.ManagedType.GetFields(
                BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance
            )
            .Where(x => x.GetCustomAttribute<UPropertyAttribute>() is not null)
            .ToImmutableArray();

        if (!ReferenceEquals(value.Parameters.ScriptStruct, method.Parameter))
        {
            throw new InvalidOperationException("Invalid evolution method!");
        }

        var getMethodDeclaration = typeof(FInstancedStruct).GetMethod(
            nameof(FInstancedStruct.Get),
            BindingFlags.Public | BindingFlags.Instance
        );
        var genericMethod = getMethodDeclaration!.MakeGenericMethod(method.Parameter.ManagedType);
        var instance = genericMethod.Invoke(value.Parameters.ScriptStruct, null)!;

        var additionalParameters = dataParameters.Select(x => x.GetValue(instance)!.ToString());

        return $"{value.Species},{value.Method},{string.Join(",", additionalParameters)}";
    }

    object? IPbsConverter.GetCsvValue(
        string input,
        PbsScalarDescriptor scalarDescriptor,
        string? sectionName
    )
    {
        return GetCsvValue(input, scalarDescriptor, sectionName);
    }

    public EvolutionConditionInfo GetCsvValue(
        string input,
        PbsScalarDescriptor scalarDescriptor,
        string? sectionName
    )
    {
        return ExtractEvolutionConditionInfo(input);
    }

    private static EvolutionConditionInfo ExtractEvolutionConditionInfo(string input)
    {
        var data = input.Split(",");
        ArgumentOutOfRangeException.ThrowIfLessThan(data.Length, 2, nameof(data));

        var species = new FName(data[0]);
        var methodName = new FName(data[1]);

        var handle = new FEvolutionHandle(methodName);
        if (!handle.IsValid)
        {
            return new EvolutionConditionInfo(species, methodName);
        }

        var method = handle.Entry;
        if (method.Parameter?.ManagedType is null)
        {
            return new EvolutionConditionInfo(species, methodName);
        }

        var dataParameters = method
            .Parameter.ManagedType.GetFields(
                BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance
            )
            .Where(p => p.GetCustomAttribute<UPropertyAttribute>() is not null)
            .ToImmutableArray();

        ArgumentOutOfRangeException.ThrowIfLessThan(
            data.Length,
            dataParameters.Length + 2,
            nameof(data)
        );

        var evolutionData = Activator.CreateInstance(method.Parameter.ManagedType);
        foreach (var (key, value) in dataParameters.Zip(data.Skip(2), (x, y) => (x, y)))
        {
            if (key.FieldType == typeof(string))
            {
                key.SetValue(evolutionData, value);
            }
            else if (
                key
                    .FieldType.GetInterfaces()
                    .Any(x => x.IsGenericType && x.GetGenericTypeDefinition() == typeof(INumber<>))
            )
            {
                var parseMethod = typeof(EvolutionMethodConverter)
                    .GetMethod(nameof(ParseNumber), BindingFlags.Static | BindingFlags.NonPublic)!
                    .MakeGenericMethod(key.FieldType);
                key.SetValue(evolutionData, parseMethod.Invoke(null, [value]));
            }
            else if (
                key
                    .FieldType.GetInterfaces()
                    .Any(x =>
                        x.IsGenericType && x.GetGenericTypeDefinition() == typeof(IDataHandle<>)
                    )
            )
            {
                key.SetValue(
                    evolutionData,
                    Activator.CreateInstance(key.FieldType, new FName(value))
                );
            }
            else
            {
                key.SetValue(evolutionData, Activator.CreateInstance(key.FieldType, value));
            }
        }

        var makeMethod = typeof(FInstancedStruct)
            .GetMethods()
            .Single(x =>
                x is { Name: nameof(FInstancedStruct.Make), IsGenericMethod: true }
                && x.GetParameters().Length == 1
            )
            .MakeGenericMethod(method.Parameter.ManagedType);

        return new EvolutionConditionInfo(
            species,
            methodName,
            (FInstancedStruct)makeMethod.Invoke(null, [evolutionData])!
        );
    }

    private static T ParseNumber<T>(string value)
        where T : INumber<T>
    {
        return T.Parse(value, null);
    }
}
