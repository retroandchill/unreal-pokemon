using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using LanguageExt;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

[UStruct]
[CreateStructView]
public readonly partial struct FLevelUpMove
{
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    private readonly bool _evolutionMove;

    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [UMetaData("ClampMin", "1")]
    [UIMin("1")]
    [EditCondition($"!{nameof(_evolutionMove)}")]
    [EditConditionHides]
    private readonly int _level;

    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [AsValue]
    public FMoveHandle Move { get; init; }

    private FLevelUpMove(int level, FMoveHandle move)
    {
        _evolutionMove = false;
        _level = level;
        Move = move;
    }

    private FLevelUpMove(FMoveHandle move)
    {
        _evolutionMove = true;
        Move = move;
        _level = 0;
    }

    public static FLevelUpMove LevelUp(int level, FMoveHandle move) => new(level, move);

    public static FLevelUpMove Evolution(FMoveHandle move) => new(move);

    public void Match(Action<int> onLevelUp, Action onEvolution)
    {
        if (_evolutionMove)
        {
            onEvolution();
        }
        else
        {
            onLevelUp(_level);
        }
    }

    public T Match<T>(Func<int, T> onLevelUp, Func<T> onEvolution)
    {
        return _evolutionMove ? onEvolution() : onLevelUp(_level);
    }
}

[UStruct]
[CreateStructView]
public readonly partial struct FEvolutionCondition
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [AsValue]
    public FSpeciesHandle Species { get; init; }

    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [AsValue]
    public FEvolutionHandle Method { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere | PropertyFlags.Instanced
    )]
    public FInstancedStruct Parameters { get; init; }

    public bool IsValid =>
        Method.IsValid && ReferenceEquals(Method.Entry.Parameters, Parameters.StructType);

    public T GetData<T>()
        where T : struct, MarshalledStruct<T>
    {
        return TryGetData<T>(out var data)
            ? data
            : throw new InvalidOperationException(
                $"Evolution condition data is not of type {typeof(T)}"
            );
    }

    public bool TryGetData<T>(out T data)
        where T : struct, MarshalledStruct<T>
    {
        if (IsValid)
        {
            return Parameters.TryGet(out data);
        }

        data = default;
        return false;
    }
}

[UStruct]
[CreateStructView]
public readonly partial struct FSpecies() : IGameDataEntry
{
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Identification"
    )]
    public required FName ID { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere,
        Category = "Identification"
    )]
    public int RowIndex { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public Option<FText> FormName { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Basic"
    )]
    public IReadOnlyList<FTypeHandle> Types { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Stats"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public IReadOnlyDictionary<FMainStatHandle, int> BaseStats { get; init; } =
        new Dictionary<FMainStatHandle, int>()
        {
            [FStat.HP] = 1,
            [FStat.ATTACK] = 1,
            [FStat.DEFENSE] = 1,
            [FStat.SPECIAL_ATTACK] = 1,
            [FStat.SPECIAL_DEFENSE] = 1,
            [FStat.SPEED] = 1,
        };

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Basic"
    )]
    public FGenderRatioHandle GenderRatio { get; init; } = FGenderRatio.Female50Percent;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Stats"
    )]
    public FGrowthRateHandle GrowthRate { get; init; } = FGrowthRate.Medium;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Drops"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int BaseExp { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "EV Yield",
        Category = "Stats"
    )]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public IReadOnlyDictionary<FMainStatHandle, int> EVYield { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Capture"
    )]
    [field: ClampMin("3")]
    [field: UIMin("3")]
    public int CatchRate { get; init; } = 255;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Basic"
    )]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public int BaseHappiness { get; init; } = 70;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Abilities"
    )]
    public IReadOnlyList<FAbilityHandle> Abilities { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Abilities"
    )]
    public IReadOnlyList<FAbilityHandle> HiddenAbilities { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Moves"
    )]
    public IReadOnlyList<FLevelUpMove> Moves { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Moves"
    )]
    public IReadOnlyList<FMoveHandle> TutorMoves { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Moves"
    )]
    public IReadOnlyList<FMoveHandle> EggMoves { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Breeding"
    )]
    public IReadOnlyList<FEggGroupHandle> EggGroups { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Breeding"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int HatchSteps { get; init; } = 1;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Breeding"
    )]
    public FItemHandle Incense { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Breeding"
    )]
    public IReadOnlyList<FSpeciesHandle> Offspring { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Evolution"
    )]
    public IReadOnlyList<FEvolutionCondition> Evolutions { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public float Height { get; init; } = 1;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public float Weight { get; init; } = 1;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    public FBodyColorHandle BodyColor { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    public FBodyShapeHandle BodyShape { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    public FHabitatHandle Habitat { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    public FText Category { get; init; } = "???";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Pokédex"
    )]
    public FText Pokedex { get; init; } = "???";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Drops"
    )]
    public IReadOnlyList<FItemHandle> WildHoldItemCommon { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Drops"
    )]
    public IReadOnlyList<FItemHandle> WildHoldItemUncommon { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Drops"
    )]
    public IReadOnlyList<FItemHandle> WildHoldItemRare { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int Generation { get; init; } = 1;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }
}

[UClass]
[GameDataRepository<FSpecies>]
[UsedImplicitly]
public partial class USpeciesRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Species))]
public readonly partial record struct FSpeciesHandle;
