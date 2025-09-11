using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public record AbilityInfo
{
    [PbsKey]
    public required FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsLocalizedText("PokemonAbilities", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    [PbsLocalizedText("PokemonAbilities", "{0}_Description")]
    public FText Description { get; init; } = "???";

    [PbsName("Flags")]
    [PbsGameplayTag("Data.Ability", Create = true)]
    public FGameplayTagContainer Tags { get; init; } = new() { GameplayTags = [], ParentTags = [] };
}
