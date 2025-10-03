using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Pbs.Converters;

/// <summary>
/// A sealed class that provides functionality for converting
/// <see cref="UnrealSharp.GameplayTags.FGameplayTag"/> types
/// to and from string representations in CSV serialization
/// within the PBS schema.
/// </summary>
public sealed class ItemPocketConverter : IPbsConverter<FGameplayTag>
{
    private const string DefaultValue = "1";

    /// <inheritdoc />
    public Type Type => typeof(FGameplayTag);

    string IPbsConverter.WriteCsvValue(object? value, PbsScalarDescriptor schema, string? sectionName)
    {
        return WriteCsvValue((FGameplayTag)value!, schema, sectionName);
    }

    /// <inheritdoc />
    public string WriteCsvValue(FGameplayTag value, PbsScalarDescriptor schema, string? sectionName)
    {
        return ExportGameplayTag(value);
    }

    private static string ExportGameplayTag(FGameplayTag value)
    {
        if (!value.IsValid)
        {
            return DefaultValue;
        }

        var targetTag = UObject
            .GetDefault<UPokemonEditorSettings>()
            .PocketNumberToGameplayTag.Find(x => x.Value == value)
            .Select(x => x.Key);

        return targetTag.Match(x => x.ToString(), () => DefaultValue);
    }

    object IPbsConverter.GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName)
    {
        return GetCsvValue(input, scalarDescriptor, sectionName);
    }

    /// <inheritdoc />
    public FGameplayTag GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName)
    {
        return ExtractGameplayTag(input);
    }

    private static FGameplayTag ExtractGameplayTag(string input)
    {
        var index = int.Parse(input);
        ArgumentOutOfRangeException.ThrowIfLessThan(index, 1, nameof(index));
        return UObject.GetDefault<UPokemonEditorSettings>().PocketNumberToGameplayTag.GetValueOrDefault(index);
    }
}
