using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using Pokemon.Editor.Serialization.Pbs.Converters;
using UnrealSharp;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public readonly record struct EvYield(FName Stat, [PbsRange<int>(1)] int Amount);

public record struct LevelUpMoveInfo([PbsRange<int>(0)] int Level, FName Move);

[PbsScalar<EvolutionMethodConverter>]
public record struct EvolutionConditionInfo(
    FName Species,
    FName Method,
    FInstancedStruct Parameters = default
);

public record SpeciesInfo
{
    [PbsKey]
    public FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsLocalizedText("PokemonSpecies", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    [PbsLocalizedText("PokemonSpecies", "{0}_FormName")]
    public FText FormName { get; init; } = FText.None;

    [PbsLength(Min = 1)]
    public IReadOnlyList<FName> Types { get; init; } = [new("NORMAL")];

    [PbsScalar<BaseStatsConverter>]
    public IReadOnlyDictionary<FName, int> BaseStats { get; init; }

    public FName GenderRatio { get; init; } = FGenderRatio.Female50Percent;

    public FName GrowthRate { get; init; } = FGrowthRate.Medium;

    public int BaseExp { get; init; } = 100;

    [PbsName("EVs")]
    public IReadOnlyList<EvYield> EVYield { get; init; } = [];

    [PbsRange<int>(3, 255)]
    public int CatchRate { get; init; } = 255;

    [PbsRange<int>(0, 255)]
    public int BaseHappiness { get; init; } = 70;

    [PbsLength(Max = 2)]
    public IReadOnlyList<FName> Abilities { get; init; } = [];

    public IReadOnlyList<FName> HiddenAbilities { get; init; } = [];

    public IReadOnlyList<LevelUpMoveInfo> Moves { get; init; } = [];

    public IReadOnlyList<FName> TutorMoves { get; init; } = [];

    public IReadOnlyList<FName> EggMoves { get; init; } = [];

    public IReadOnlyList<FName> EggGroups { get; init; } = [FEggGroup.Undiscovered];

    public int HatchSteps { get; init; } = 1;

    public FName Incense { get; init; }

    public IReadOnlyList<FName> Offspring { get; init; } = [];

    [PbsAllowMultiple]
    [PbsName("Evolution")]
    public IReadOnlyList<EvolutionConditionInfo> Evolutions { get; init; } = [];

    [PbsRange<float>(0.1f)]
    public float Height { get; init; } = 0.1f;

    [PbsRange<float>(0.1f)]
    public float Weight { get; init; } = 0.1f;

    [PbsName("Color")]
    public FName BodyColor { get; init; } = FBodyColor.Red;

    [PbsName("Shape")]
    public FName BodyShape { get; init; } = FBodyShape.Head;

    public FName Habitat { get; init; }

    [PbsLocalizedText("PokemonSpecies", "{0}_Category")]
    public FText Category { get; init; } = "???";

    [PbsLocalizedText("PokemonSpecies", "{0}_Pokedex")]
    public FText Pokedex { get; init; } = "???";

    public IReadOnlyList<FName> WildHoldItemCommon { get; init; } = [];

    public IReadOnlyList<FName> WildHoldItemUncommon { get; init; } = [];

    public IReadOnlyList<FName> WildHoldItemRare { get; init; } = [];

    [PbsRange<int>(1)]
    public int Generation { get; init; } = 1;

    [PbsName("Flags")]
    [PbsGameplayTag("Data.Species", Create = true, Separator = "_")]
    public FGameplayTagContainer Tags { get; init; }

    // ReSharper disable once ConvertConstructorToMemberInitializers
    public SpeciesInfo()
    {
        BaseStats = new Dictionary<FName, int>
        {
            [FStat.HP] = 1,
            [FStat.ATTACK] = 1,
            [FStat.DEFENSE] = 1,
            [FStat.SPECIAL_ATTACK] = 1,
            [FStat.SPECIAL_DEFENSE] = 1,
            [FStat.SPEED] = 1,
        };
    }
}
