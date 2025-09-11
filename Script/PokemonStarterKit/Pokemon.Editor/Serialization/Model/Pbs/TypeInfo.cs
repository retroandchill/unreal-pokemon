using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public record TypeInfo
{
    [PbsKey]
    public required FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsLocalizedText("PokemonTypes", "{0}_Name")]
    public FText Name { get; init; } = "Unnamed";

    public bool IsSpecialType { get; init; }

    public bool IsPseudoType { get; init; }

    public IReadOnlyList<FName> Weaknesses { get; init; } = [];

    public IReadOnlyList<FName> Resistances { get; init; } = [];

    public IReadOnlyList<FName> Immunities { get; init; } = [];

    [PbsName("Flags")]
    [PbsGameplayTag("Data.Type", Create = true)]
    public FGameplayTagContainer Tags { get; init; } = new() { GameplayTags = [], ParentTags = [] };
}
