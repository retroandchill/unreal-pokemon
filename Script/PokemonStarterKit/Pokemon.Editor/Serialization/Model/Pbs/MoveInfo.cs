using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public record MoveInfo
{
    [PbsKey]
    public required FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsLocalizedText("PokemonMoves", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    [PbsLocalizedText("PokemonMoves", "{0}_Description")]
    public FText Description { get; init; } = "???";
    
    public FName Type { get; init; }

    public EDamageCategory Category { get; init; } = EDamageCategory.Status;

    [PbsRange<int>(0)]
    public int Power { get; init; }

    [PbsRange<int>(0, 100)]
    public int Accuracy { get; init; } = 100;

    [PbsRange<int>(1)]
    public int TotalPP { get; init; } = 5;

    public int Priority { get; init; }
    
    public FName Target { get; init; }

    [PbsGameplayTag(IdentifierConstants.FunctionCodeTag, Create = true)]
    public FGameplayTag FunctionCode { get; init; }

    [PbsRange<int>(0)]
    public int EffectChance { get; init; }

    [PbsName("Flags")]
    [PbsGameplayTag("Data.Move", Create = true)]
    public FGameplayTagContainer Tags { get; init; } = new() { GameplayTags = [], ParentTags = [] };
}
