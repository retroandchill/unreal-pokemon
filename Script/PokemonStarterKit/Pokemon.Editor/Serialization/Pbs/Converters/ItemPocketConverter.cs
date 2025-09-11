using Pokemon.Editor.Serialization.Pbs;
using Pokemon.Editor.Serialization.Pbs.Converters;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serializers.Pbs.Converters;

public sealed class ItemPocketConverter : PbsConverterBase<FGameplayTag>
{
    private const string DefaultValue = "1";

    public override string WriteCsvValue(
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

    public override FGameplayTag GetCsvValue(
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
