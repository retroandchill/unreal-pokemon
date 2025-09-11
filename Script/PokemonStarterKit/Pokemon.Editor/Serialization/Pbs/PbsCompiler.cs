using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Numerics;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using CsvHelper;
using CsvHelper.Configuration;
using GameDataAccessTools.Core.Utilities;
using Pokemon.Editor.Serialization.Pbs.Utilities;
using UnrealSharp;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Pbs;

public readonly record struct PbsSectionData(
    string Name,
    int Index,
    IReadOnlyDictionary<string, List<string>> Contents
);

public static partial class PbsCompiler
{
    private const string NameNone = "None";

    public static IEnumerable<PbsSectionData> EachFileSection(
        TextReader reader,
        PbsSchema? schema = null
    )
    {
        var sectionIndex = 0;
        var lineNumber = 1;
        string? sectionName = null;
        var lastSection = new Dictionary<string, List<string>>();
        var line = reader.ReadLine();
        while (line is not null)
        {
            if (!line.StartsWith('#') && !string.IsNullOrWhiteSpace(line))
            {
                line = PrepLine(line);

                if (TryGetSectionHeader(line, out var newSection))
                {
                    if (sectionName is not null)
                    {
                        yield return new PbsSectionData(sectionName, sectionIndex, lastSection);
                        sectionIndex++;
                    }

                    sectionName = newSection;
                    lastSection = new Dictionary<string, List<string>>();
                }
                else
                {
                    if (sectionName is null)
                    {
                        throw new InvalidOperationException(
                            $"Unexpected line {lineNumber} in file: {line}\nExpected a section at the beginning of the file.\nThis error may also occur if the file was not saved in UTF-8."
                        );
                    }

                    if (!TryGetEntry(line, out var key, out var value))
                    {
                        throw new InvalidOperationException(
                            $"Unexpected line {lineNumber} in file: {line}\nBad line syntax (expected syntax like XXX=YYY)."
                        );
                    }

                    if (
                        schema is not null
                        && schema.TryGetField(key, out var property)
                        && property.Repeat == RepeatMode.KeyRepeat
                    )
                    {
                        if (!lastSection.TryGetValue(key, out var existingValue))
                        {
                            existingValue = [];
                            lastSection[key] = existingValue;
                            continue;
                        }

                        existingValue.Add(value.TrimEnd());
                    }
                    else
                    {
                        lastSection[key] = [value.TrimEnd()];
                    }
                }
            }

            line = reader.ReadLine();
            lineNumber++;
        }

        if (sectionName is not null)
            yield return new PbsSectionData(sectionName, sectionIndex, lastSection);
    }

    private static string PrepLine(string line)
    {
        return PreCommentRegex().Replace(line, string.Empty).Trim();
    }

    [GeneratedRegex(@"\s*\#.*$", RegexOptions.Compiled)]
    private static partial Regex PreCommentRegex();

    private static bool TryGetSectionHeader(
        string line,
        [NotNullWhen(true)] out string? sectionName
    )
    {
        var sectionHeaderRegex = SectionHeaderRegex();
        var match = sectionHeaderRegex.Match(line);
        if (match.Success)
        {
            sectionName = match.Groups[1].Value;
            return true;
        }

        sectionName = null;
        return false;
    }

    [GeneratedRegex(@"^\s*\[\s*(.*)\s*\]\s*$")]
    private static partial Regex SectionHeaderRegex();

    private static bool TryGetEntry(
        string line,
        [NotNullWhen(true)] out string? key,
        [NotNullWhen(true)] out string? value
    )
    {
        var valueEntryRegex = SectionEntryRegex();
        var match = valueEntryRegex.Match(line);
        if (match.Success)
        {
            key = match.Groups[1].Value;
            value = match.Groups[2].Value;
            return true;
        }

        key = null;
        value = null;
        return false;
    }

    [GeneratedRegex(@"^\s*(\w+)\s*=\s*(.*)$")]
    private static partial Regex SectionEntryRegex();

    public static IReadOnlyList<string> SplitCsvLine(string input)
    {
        var config = new CsvConfiguration(CultureInfo.InvariantCulture)
        {
            Delimiter = ",",
            Mode = CsvMode.RFC4180,
            Escape = '\\',
            HasHeaderRecord = false,
        };

        using var csv = new CsvReader(new StringReader(input), config);
        csv.Read();
        return csv.Parser.Record ?? [];
    }

    public static object? GetCsvRecord(string input, PbsFieldDescriptor schema, string? sectionName)
    {
        var targetType = schema.TargetProperty.PropertyType;
        if (schema.IsScalar)
        {
            return GetCsvValue(input, schema.Elements.Single(), sectionName);
        }

        var subLists =
            schema.TargetProperty.TryGetCollectionType(out var innerType)
            && schema is { Repeat: RepeatMode.CsvRepeat, Elements.Length: > 1 };
        var values = SplitCsvLine(input);
        var idx = -1;
        var result = new List<object?>();
        while (true)
        {
            var record = new object?[schema.Elements.Length];
            foreach (var (element, i) in schema.Elements.Select((x, i) => (x, i)))
            {
                idx++;
                if (element.IsOptional && string.IsNullOrWhiteSpace(values[idx]))
                {
                    record[i] = null;
                    continue;
                }

                record[i] = GetCsvValue(values[idx], element, sectionName);
            }

            if (subLists)
            {
                result.Add(Activator.CreateInstance(innerType!, record));
            }
            else
            {
                result.AddRange(record);
            }

            if (schema.Repeat != RepeatMode.CsvRepeat || idx >= values.Count - 1)
                break;
        }

        return targetType.TryGetCollectionFactory(out var factory)
            ? factory(result)
            : Activator.CreateInstance(targetType, result.ToArray());
    }

    private static object? GetCsvValue(
        string input,
        PbsScalarDescriptor schema,
        string? sectionName
    )
    {
        if (schema.ScalarConverter is not null)
        {
            return schema.ScalarConverter.GetCsvValue(input, schema, sectionName);
        }

        var schemaType = schema.Type;
        if (schemaType.IsGenericType && schemaType.GetGenericTypeDefinition() == typeof(Nullable<>))
        {
            schemaType = schemaType.GetGenericArguments()[0];
        }

        if (schemaType == typeof(bool))
        {
            return ParseBool(input);
        }

        if (schemaType == typeof(string))
        {
            return input;
        }

        if (schemaType == typeof(FName))
        {
            return input.Equals(NameNone, StringComparison.OrdinalIgnoreCase)
                ? FName.None
                : new FName(GetGameplayTagName(input, schema));
        }

        if (schemaType == typeof(FText))
        {
            if (sectionName is null || !schema.LocalizedTextNamespace.HasValue)
            {
                return FText.FromLocalizedString(input);
            }

            var localizationKey = string.Format(
                schema.LocalizedTextNamespace.Value.KeyFormat,
                sectionName
            );

            return FText.Localized(
                schema.LocalizedTextNamespace.Value.Namespace,
                localizationKey,
                input
            );
        }

        if (schemaType == typeof(FGameplayTag))
        {
            if (input.Equals(NameNone, StringComparison.OrdinalIgnoreCase))
            {
                return new FGameplayTag();
            }

            var tagName = GetGameplayTagName(input, schema);
            return schema.CreateNewGameplayTag
                ? FGameplayTag.GetOrCreate(tagName)
                : new FGameplayTag(tagName);
        }

        if (schemaType.IsEnum)
        {
            return Enum.Parse(schemaType, input, true);
        }

        if (
            schemaType
                .GetInterfaces()
                .Any(i => i.IsGenericType && i.GetGenericTypeDefinition() == typeof(INumber<>))
        )
        {
            var parseNumber = typeof(PbsCompiler)
                .GetMethod(nameof(ParseNumber), BindingFlags.Static | BindingFlags.Public)!
                .MakeGenericMethod(schemaType);
            return parseNumber.Invoke(null, [input, schema.NumericBounds, null])!;
        }

        throw new InvalidOperationException($"Unsupported scalar type {schema.Type}");
    }

    private static string GetGameplayTagName(string input, PbsScalarDescriptor scalarDescriptor)
    {
        if (scalarDescriptor.GameplayTagNamespace is null)
        {
            return input;
        }

        var tagLeaf = scalarDescriptor.GameplayTagSeparator is not null
            ? input.Replace(scalarDescriptor.GameplayTagSeparator, ".")
            : input;
        return $"{scalarDescriptor.GameplayTagNamespace}.{tagLeaf}";
    }

    public static bool ParseBool(string input)
    {
        var trueRegex = TrueRegex();
        var falseRegex = FalseRegex();

        if (trueRegex.IsMatch(input))
        {
            return true;
        }

        if (falseRegex.IsMatch(input))
        {
            return false;
        }

        throw new InvalidOperationException($"Failed to parse boolean {input}");
    }

    [GeneratedRegex("^(?:1|TRUE|YES|Y)$", RegexOptions.IgnoreCase, "en-US")]
    private static partial Regex TrueRegex();

    [GeneratedRegex("^(?:0|FALSE|NO|N)$", RegexOptions.IgnoreCase, "en-US")]
    private static partial Regex FalseRegex();

    public static T ParseNumber<T>(
        string input,
        NumericBounds<T> bounds = default,
        IFormatProvider? provider = null
    )
        where T : struct, INumber<T>
    {
        if (!T.TryParse(input, provider, out var result))
        {
            throw new InvalidOperationException($"Failed to parse number {input}");
        }

        if (bounds.Min.HasValue && result.CompareTo(bounds.Min.Value) < 0)
        {
            throw new InvalidOperationException($"Number {input} is below minimum {bounds.Min}");
        }

        if (bounds.Max.HasValue && result.CompareTo(bounds.Max.Value) > 0)
        {
            throw new InvalidOperationException($"Number {input} is above maximum {bounds.Max}");
        }

        return result;
    }

    public static IReadOnlyDictionary<string, T> CompilePbsFile<T>(TextReader reader)
    {
        var result = new Dictionary<string, T>();
        var schema = PbsMetamodel.GetSchema<T>();
        foreach (var (sectionName, i, contents) in EachFileSection(reader, schema))
        {
            if (result.ContainsKey(sectionName))
            {
                throw new InvalidOperationException($"Duplicate section {sectionName} in file");
            }

            var instance = Activator.CreateInstance<T>();
            foreach (var (_, field) in schema.Fields)
            {
                if (field.IsIdentifier)
                {
                    field.TargetProperty.SetValue(
                        instance,
                        GetCsvRecord(sectionName, field, sectionName)
                    );
                    continue;
                }

                if (field.IsRowIndex)
                {
                    field.TargetProperty.SetValue(instance, i);
                    continue;
                }

                if (!contents.TryGetValue(field.KeyName, out var contentValue))
                    continue;

                if (field.Repeat == RepeatMode.KeyRepeat)
                {
                    var value = contentValue.Select(x => GetCsvRecord(x, field, sectionName));
                    if (!field.TargetProperty.PropertyType.TryGetCollectionFactory(out var factory))
                    {
                        throw new InvalidOperationException(
                            $"Unsupported collection type {field.TargetProperty.PropertyType}"
                        );
                    }

                    field.TargetProperty.SetValue(instance, factory(value));
                }
                else
                {
                    field.TargetProperty.SetValue(
                        instance,
                        GetCsvRecord(contentValue.Single(), field, sectionName)
                    );
                }
            }

            result.Add(sectionName, instance);
        }

        return result;
    }

    public static void WritePbs<T>(IEnumerable<T> entries, TextWriter textWriter)
    {
        var schema = PbsMetamodel.GetSchema<T>();
        var keyField = schema.Fields.Values.Single(x => x.IsIdentifier);
        var defaultValue = Activator.CreateInstance<T>();
        textWriter.WriteLine("# See the documentation on the wiki to learn how to edit this file.");
        foreach (var entry in entries)
        {
            textWriter.WriteLine("#-------------------------------");
            var sectionName = WriteCsvRecord(keyField.TargetProperty.GetValue(entry), keyField);
            textWriter.WriteLine($"[{sectionName}]");

            foreach (var field in schema.Fields.Values)
            {
                if (field.IsIdentifier || field.IsRowIndex)
                    continue;

                var value = field.TargetProperty.GetValue(entry);
                if (
                    value is null
                    || !ShouldWriteValue(value, field.TargetProperty.GetValue(defaultValue))
                )
                    continue;

                if (field.Repeat == RepeatMode.KeyRepeat && value is IEnumerable enumerable)
                {
                    foreach (var item in enumerable)
                    {
                        textWriter.WriteLine(
                            $"{field.KeyName} = {WriteCsvRecord(item, field, sectionName)}"
                        );
                    }
                }
                else
                {
                    textWriter.WriteLine(
                        $"{field.KeyName} = {WriteCsvRecord(value, field, sectionName)}"
                    );
                }
            }
        }
    }

    private static bool ShouldWriteValue(object value, object? defaultValue)
    {
        return value switch
        {
            FGameplayTagContainer gameplayTagContainer => gameplayTagContainer.GameplayTags.Any(),
            IEnumerable enumerable => enumerable.Cast<object>().Any(),
            _ => !value.Equals(defaultValue),
        };
    }

    private static string WriteCsvRecord(
        object? record,
        PbsFieldDescriptor schema,
        string? sectionName = null
    )
    {
        if (schema.IsScalar)
        {
            return WriteCsvValue(record, schema.Elements.Single(), sectionName);
        }

        if (schema.Repeat != RepeatMode.CsvRepeat)
        {
            return string.Join(
                ",",
                DeconstructComplexType(record, schema.TargetProperty.PropertyType, schema)
            );
        }

        switch (record)
        {
            case FGameplayTagContainer gameplayTagContainer:
            {
                var scalar = schema.Elements.Single();
                return string.Join(
                    ",",
                    gameplayTagContainer.GameplayTags.Select(x =>
                        WriteCsvValue(x, scalar, sectionName)
                    )
                );
            }
            case IEnumerable enumerable
                when schema.Elements.Length == 1 && schema.Elements[0].Type.IsScalarType():
            {
                var scalar = schema.Elements[0];
                return string.Join(
                    ",",
                    enumerable.Cast<object>().Select(x => WriteCsvValue(x, scalar, sectionName))
                );
            }
            case IEnumerable enumerable:
            {
                if (!schema.TargetProperty.TryGetCollectionType(out var elementType))
                {
                    throw new InvalidOperationException("Could not get collection type");
                }

                return string.Join(
                    ",",
                    enumerable
                        .Cast<object>()
                        .SelectMany(x => DeconstructComplexType(x, elementType, schema))
                );
            }
            default:
                throw new InvalidOperationException(
                    $"Invalid repeating CSV type {record?.GetType().ToString() ?? "null"}"
                );
        }
    }

    private static IEnumerable<string> DeconstructComplexType(
        object? record,
        Type targetType,
        PbsFieldDescriptor schema,
        string? sectionName = null
    )
    {
        var deconstructMethod = targetType
            .GetMethods(BindingFlags.CreateInstance | BindingFlags.Public)
            .Single(m => IsValidDeconstructMethod(m, schema));

        var args = new object?[schema.Elements.Length];

        deconstructMethod.Invoke(record, args);
        return schema.Elements.Select((e, i) => WriteCsvValue(args[i], e, sectionName));
    }

    private static bool IsValidDeconstructMethod(MethodInfo method, PbsFieldDescriptor schema)
    {
        if (method.Name != "Deconstruct")
            return false;

        if (method.ReturnType != typeof(void))
            return false;

        var parameters = method.GetParameters();
        if (parameters.Length != schema.Elements.Length)
            return false;

        for (var i = 0; i < parameters.Length; i++)
        {
            var parameter = parameters[i];
            var element = schema.Elements[i];
            if (parameter.ParameterType != element.Type)
                return false;
            if (!parameter.IsOut)
                return false;
        }

        return true;
    }

    private static string WriteCsvValue(
        object? value,
        PbsScalarDescriptor schema,
        string? sectionName
    )
    {
        if (schema.ScalarConverter is not null)
        {
            return schema.ScalarConverter.WriteCsvValue(value, schema, sectionName);
        }

        return value switch
        {
            FGameplayTag or FName => StripGameplayTagNamespace(
                value.ToString()!,
                schema.GameplayTagNamespace
            ),
            FText text => WriteLocalizedString(text, schema.LocalizedTextNamespace, sectionName),
            bool b => b ? "true" : "false",
            _ => value?.ToString() ?? throw new ArgumentNullException(nameof(value)),
        };
    }

    private static string StripGameplayTagNamespace(string value, string? namespacePrefix)
    {
        ArgumentNullException.ThrowIfNull(value);
        return namespacePrefix is not null && value.StartsWith($"{namespacePrefix}.")
            ? value[$"{namespacePrefix}.".Length..]
            : value;
    }

    private static string WriteLocalizedString(
        FText value,
        LocalizedTextNamespace? localizedTextNamespace,
        string? sectionName
    )
    {
        if (sectionName is null || !localizedTextNamespace.HasValue)
            return value.ToLocalizedString();

        if (
            value.TryGetNamespace(out var ns)
            && value.TryGetKey(out var key)
            && (
                ns == localizedTextNamespace.Value.Namespace
                || key == string.Format(localizedTextNamespace.Value.KeyFormat, sectionName)
            )
        )
        {
            return value.ToString();
        }

        return value.ToLocalizedString();
    }
}
