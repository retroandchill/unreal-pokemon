using System.Collections.Immutable;
using System.Reflection;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameDataAccessToolsEditor;
using ZLinq;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

[UClass]
[UsedImplicitly]
public class UGameDataEntryPbsSerializer : UCSGameDataEntrySerializer
{
    protected override bool Supports(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        var defaultObject = repositoryClass.DefaultObject;
        var entryStruct = defaultObject.EntryStruct;
        try
        {
            var matchingType = repositoryClass
                .DefaultObject.GetType()
                .GetInterfaces()
                .Where(i =>
                    i.IsGenericType && i.GetGenericTypeDefinition() == typeof(IGameDataRepository<>)
                )
                .Select(i => i.GetGenericArguments()[0])
                .SingleOrDefault(t =>
                {
                    var method = typeof(UGameDataEntryPbsSerializer)
                        .GetMethod(
                            nameof(MatchesScriptStruct),
                            BindingFlags.NonPublic | BindingFlags.Static
                        )!
                        .MakeGenericMethod(t);
                    return (bool)method.Invoke(null, [entryStruct])!;
                });

            return matchingType is not null
                && PbsSerializer.Instance.HasSerializerFor(matchingType);
        }
        catch (Exception e)
        {
            LogPokemonEditor.LogError(
                $"An unexpected error occurred while trying to determine if the PBS serializer supports the repository class: {e.Message}"
            );
            return false;
        }
    }

    protected override FText GetFormatName()
    {
        return FText.Localized("Pokemon.Editor", "PbsFormatName", "PBS");
    }

    protected override string GetFileExtensionText()
    {
        return "PBS file |*.txt|";
    }

    protected override bool Serialize(
        string filePath,
        UAssetGameDataRepository repository,
        out string errorMessage
    )
    {
        try
        {
            var entryStruct = repository.EntryStruct;
            var matchingType = repository
                .GetType()
                .GetInterfaces()
                .Where(i =>
                    i.IsGenericType && i.GetGenericTypeDefinition() == typeof(IGameDataRepository<>)
                )
                .Select(i => i.GetGenericArguments()[0])
                .Single(t =>
                {
                    var method = typeof(UGameDataEntryPbsSerializer)
                        .GetMethod(
                            nameof(MatchesScriptStruct),
                            BindingFlags.NonPublic | BindingFlags.Static
                        )!
                        .MakeGenericMethod(t);
                    return (bool)method.Invoke(null, [entryStruct])!;
                });

            var serializeMethod = typeof(UGameDataEntryPbsSerializer)
                .GetMethod(nameof(SerializeInternal), BindingFlags.NonPublic | BindingFlags.Static)!
                .MakeGenericMethod(matchingType);
            serializeMethod.Invoke(this, [filePath, repository]);
            errorMessage = string.Empty;
            return true;
        }
        catch (Exception e)
        {
            if (e is TargetInvocationException { InnerException: not null } tie)
            {
                e = tie.InnerException;
            }

            errorMessage = e.Message;
            return false;
        }
    }

    protected override bool Deserialize(
        string filePath,
        UAssetGameDataRepository repository,
        out string errorMessage
    )
    {
        try
        {
            var entryStruct = repository.EntryStruct;
            var matchingType = repository
                .GetType()
                .GetInterfaces()
                .Where(i =>
                    i.IsGenericType && i.GetGenericTypeDefinition() == typeof(IGameDataRepository<>)
                )
                .Select(i => i.GetGenericArguments()[0])
                .Single(t =>
                {
                    var method = typeof(UGameDataEntryPbsSerializer)
                        .GetMethod(
                            nameof(MatchesScriptStruct),
                            BindingFlags.NonPublic | BindingFlags.Static
                        )!
                        .MakeGenericMethod(t);
                    return (bool)method.Invoke(null, [entryStruct])!;
                });

            var serializeMethod = typeof(UGameDataEntryPbsSerializer)
                .GetMethod(
                    nameof(DeserializeInternal),
                    BindingFlags.NonPublic | BindingFlags.Static
                )!
                .MakeGenericMethod(matchingType);
            serializeMethod.Invoke(this, [filePath, repository]);
            errorMessage = string.Empty;
            return true;
        }
        catch (Exception e)
        {
            if (e is TargetInvocationException { InnerException: not null } tie)
            {
                e = tie.InnerException;
            }

            errorMessage = e.Message;
            return false;
        }
    }

    private static bool MatchesScriptStruct<T>(UScriptStruct scriptStruct)
        where T : MarshalledStruct<T>
    {
        return scriptStruct.NativeObject == T.GetNativeClassPtr();
    }

    private static void SerializeInternal<T>(string filePath, IGameDataRepository<T> repository)
        where T : struct, IGameDataEntry, MarshalledStruct<T>
    {
        using var fileStream = File.OpenWrite(filePath);
        using var streamWriter = new StreamWriter(fileStream);

        var entries = repository.Entries.AsValueEnumerable().ToArray();

        PbsSerializer.Instance.Serialize(entries, streamWriter);
    }

    private static void DeserializeInternal<T>(string filePath, IGameDataRepository<T> repository)
        where T : struct, IGameDataEntry, MarshalledStruct<T>
    {
        using var streamReader = File.OpenText(filePath);
        var entries = PbsSerializer.Instance.Deserialize<T>(streamReader).ToArray();

        if (
            repository
                .GetType()
                .GetProperty("DataEntries", BindingFlags.NonPublic | BindingFlags.Instance)!
                .GetValue(repository)
            is not IList<T> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (var entry in entries)
        {
            dataEntries.Add(entry);
        }
    }
}
