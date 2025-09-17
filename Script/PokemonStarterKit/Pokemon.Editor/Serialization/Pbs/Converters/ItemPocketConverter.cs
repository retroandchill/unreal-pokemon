using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Pbs.Converters;

public sealed class ItemPocketConverter : IPbsConverter<FGameplayTag>
{
    private const string DefaultValue = "1";

    public Type Type => typeof(FGameplayTag);

    string IPbsConverter.WriteCsvValue(object? value, PbsScalarDescriptor schema, string? sectionName)
    {
        return WriteCsvValue((FGameplayTag)value!, schema, sectionName);
    }

    public string WriteCsvValue(
        FGameplayTag value,
        PbsScalarDescriptor schema,
        string? sectionName
    )
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

    object? IPbsConverter.GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName)
    {
        return GetCsvValue(input, scalarDescriptor, sectionName);
    }

    public FGameplayTag GetCsvValue(
        string input,
        PbsScalarDescriptor scalarDescriptor,
        string? sectionName
    )
    {
        var index = int.Parse(input);
        ArgumentOutOfRangeException.ThrowIfLessThan(index, 1, nameof(index));
        return UObject
            .GetDefault<UPokemonEditorSettings>()
            .PocketNumberToGameplayTag.GetValueOrDefault(index);
    }
}
