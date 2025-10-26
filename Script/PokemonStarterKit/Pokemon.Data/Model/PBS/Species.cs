using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Represents a structured definition for a Pokémon's level-up move, encapsulating
/// its associated move and conditions under which it is learned (e.g., level-up or evolution).
/// </summary>
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

    /// <summary>
    /// Gets the move data associated with the level-up or evolution.
    /// Represents the move learned by a Pokémon under specific conditions,
    /// such as reaching a specific level or evolving.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
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

    /// <summary>
    /// Creates and returns a new instance of the <see cref="FLevelUpMove"/> structure, representing
    /// a Pokémon's level-up move at a specific level.
    /// </summary>
    /// <param name="level">The level at which the move is learned.</param>
    /// <param name="move">The move being learned, represented as an <see cref="FMoveHandle"/> handle.</param>
    /// <returns>A new <see cref="FLevelUpMove"/> instance initialized with the specified level and move.</returns>
    public static FLevelUpMove LevelUp(int level, FMoveHandle move) => new(level, move);

    /// <summary>
    /// Creates and returns a new instance of the <see cref="FLevelUpMove"/> structure,
    /// representing a Pokémon's move learned through evolution.
    /// </summary>
    /// <param name="move">The move learned as an <see cref="FMoveHandle"/> handle during evolution.</param>
    /// <returns>A new <see cref="FLevelUpMove"/> instance initialized with the specified evolution move.</returns>
    public static FLevelUpMove Evolution(FMoveHandle move) => new(move);

    /// <summary>
    /// Executes the appropriate action or logic based on whether the move is learned through level-up or evolution.
    /// </summary>
    /// <param name="onLevelUp">An action to invoke when the move is learned at a specific level. The current level is passed as an argument.</param>
    /// <param name="onEvolution">An action to invoke when the move is learned through evolution.</param>
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

    /// <summary>
    /// Evaluates whether the given move is learned through level-up or evolution by executing
    /// the corresponding provided function and returns the result of that function.
    /// </summary>
    /// <typeparam name="T">The return type of the provided functions.</typeparam>
    /// <param name="onLevelUp">A function to execute if the move is learned through level-up, receiving the level as an argument.</param>
    /// <param name="onEvolution">A function to execute if the move is learned through evolution.</param>
    /// <returns>The result of the executed function, based on whether the move is learned through level-up or evolution.</returns>
    public T Match<T>(Func<int, T> onLevelUp, Func<T> onEvolution)
    {
        return _evolutionMove ? onEvolution() : onLevelUp(_level);
    }
}

/// <summary>
/// Represents the conditions under which a Pokémon species is eligible to evolve,
/// including the species it evolves into, the method by which the evolution occurs,
/// and any additional parameters required to fulfill the evolution criteria.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FEvolutionCondition
{
    /// <summary>
    /// Gets the handle representing the species of a Pokémon.
    /// Refers to the specific species of the Pokémon, used to identify and manage
    /// species-related data such as evolution chains, base stats, and more.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [AsValue]
    public FSpeciesHandle Species { get; init; }

    /// <summary>
    /// Represents the method by which a Pokémon undergoes evolution.
    /// Encapsulates the specific evolution mechanism, detailing how the evolution occurs,
    /// such as through level-up, use of a specific item, trade, or other conditions.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [AsValue]
    public FEvolutionHandle Method { get; init; }

    /// <summary>
    /// Gets the additional parameters required for the evolution criteria.
    /// These parameters provide specific details necessary for determining
    /// whether the evolution conditions are met, depending on the evolution method.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere | PropertyFlags.Instanced)]
    public FInstancedStruct Parameters { get; init; }

    /// <summary>
    /// Determines whether the evolution condition is valid by verifying that
    /// the evolution method is valid and the specified parameter type matches
    /// the expected parameter type for the method.
    /// </summary>
    public bool IsValid => Method.IsValid && ReferenceEquals(Method.Entry.Parameter, Parameters.StructType);

    /// <summary>
    /// Retrieves and returns the evolution condition parameters as a strongly-typed structure.
    /// </summary>
    /// <typeparam name="T">The type of structure expected, which must implement <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <returns>The parameters of the evolution condition as an instance of type <typeparamref name="T"/>.</returns>
    /// <exception cref="InvalidOperationException">Thrown if the evolution condition data is not of the specified type <typeparamref name="T"/>.</exception>
    public T GetData<T>()
        where T : struct, MarshalledStruct<T>
    {
        return TryGetData<T>(out var data)
            ? data
            : throw new InvalidOperationException($"Evolution condition data is not of type {typeof(T)}");
    }

    /// <summary>
    /// Attempts to retrieve the data encapsulated within the evolution condition parameters as the specified type.
    /// </summary>
    /// <typeparam name="T">The type of the data to retrieve. Must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="data">When this method returns, contains the data of the specified type if the operation succeeds, or the default value of the type if it fails.</param>
    /// <returns>True if the data is successfully retrieved and matches the specified type; otherwise, false.</returns>
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

/// <summary>
/// Represents the detailed definition of a Pokémon species, encapsulating its
/// identification, display, statistical, elemental types, abilities, moves,
/// growth characteristics, breeding groups, and various other species attributes.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FSpecies() : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the display name of the Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the optional form name for variant forms of the Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    public TOptional<FText> FormName { get; init; }

    /// <summary>
    /// Gets the list of types associated with this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Basic")]
    public IReadOnlyList<FTypeHandle> Types { get; init; }

    /// <summary>
    /// Gets the base stats for the Pokémon species, including HP, Attack, Defense, Special Attack, Special Defense, and Speed.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
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

    /// <summary>
    /// Gets the gender ratio of the Pokémon species, determining the probability of each gender when encountered.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Basic")]
    [AsValue]
    public FGenderRatioHandle GenderRatio { get; init; } = FGenderRatio.Female50Percent;

    /// <summary>
    /// Gets the growth rate of the Pokémon species, determining how quickly it gains experience and levels up.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    [AsValue]
    public FGrowthRateHandle GrowthRate { get; init; } = FGrowthRate.Medium;

    /// <summary>
    /// Gets the base experience points yielded when this Pokémon is defeated.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Drops")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int BaseExp { get; init; }

    /// <summary>
    /// Gets the Effort Value (EV) yield for each stat when this Pokémon is defeated.
    /// </summary>
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "EV Yield",
        Category = "Stats"
    )]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public IReadOnlyDictionary<FMainStatHandle, int> EVYield { get; init; }

    /// <summary>
    /// Gets the catch rate of the Pokémon species, determining how easy it is to catch in the wild.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Capture")]
    [field: ClampMin("3")]
    [field: UIMin("3")]
    public int CatchRate { get; init; } = 255;

    /// <summary>
    /// Gets the base happiness value of the Pokémon species when first obtained.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Basic")]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public int BaseHappiness { get; init; } = 70;

    /// <summary>
    /// Gets the list of possible regular abilities for this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Abilities")]
    public IReadOnlyList<FAbilityHandle> Abilities { get; init; }

    /// <summary>
    /// Gets the list of possible hidden abilities for this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Abilities")]
    public IReadOnlyList<FAbilityHandle> HiddenAbilities { get; init; }

    /// <summary>
    /// Gets the list of moves this Pokémon species can learn through level-up.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Moves")]
    public IReadOnlyList<FLevelUpMove> Moves { get; init; }

    /// <summary>
    /// Gets the list of moves this Pokémon species can learn through move tutors.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Moves")]
    public IReadOnlyList<FMoveHandle> TutorMoves { get; init; }

    /// <summary>
    /// Gets the list of moves this Pokémon species can learn through breeding.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Moves")]
    public IReadOnlyList<FMoveHandle> EggMoves { get; init; }

    /// <summary>
    /// Gets the list of egg groups this Pokémon species belongs to for breeding compatibility.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Breeding")]
    public IReadOnlyList<FEggGroupHandle> EggGroups { get; init; }

    /// <summary>
    /// Gets the number of steps required for this Pokémon species' egg to hatch.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Breeding")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int HatchSteps { get; init; } = 1;

    /// <summary>
    /// Gets the specific incense item required for breeding certain variant forms of this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Breeding")]
    [field: UMetaData(Metadata.AllowNone)]
    [AsValue]
    public FItemHandle Incense { get; init; }

    /// <summary>
    /// Gets the list of Pokémon species that can be produced when breeding this species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Breeding")]
    public IReadOnlyList<FSpeciesHandle> Offspring { get; init; }

    /// <summary>
    /// Gets the list of evolution conditions that determine how this Pokémon species can evolve.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Evolution")]
    public IReadOnlyList<FEvolutionCondition> Evolutions { get; init; }

    /// <summary>
    /// Gets the height of this Pokémon species in meters.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public float Height { get; init; } = 1;

    /// <summary>
    /// Gets the weight of this Pokémon species in kilograms.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    [field: ClampMin("0")]
    [field: UIMin("0")]
    public float Weight { get; init; } = 1;

    /// <summary>
    /// Gets the body color classification of this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    [AsValue]
    public FBodyColorHandle BodyColor { get; init; }

    /// <summary>
    /// Gets the body shape classification of this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    [AsValue]
    public FBodyShapeHandle BodyShape { get; init; }

    /// <summary>
    /// Gets the natural habitat classification of this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    [AsValue]
    public FHabitatHandle Habitat { get; init; }

    /// <summary>
    /// Gets the category description of this Pokémon species as shown in the Pokédex.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    public FText Category { get; init; } = "???";

    /// <summary>
    /// Gets the Pokédex entry description for this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Pokédex")]
    public FText Pokedex { get; init; } = "???";

    /// <summary>
    /// Gets the list of common items that this Pokémon species may hold when encountered in the wild.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Drops")]
    public IReadOnlyList<FItemHandle> WildHoldItemCommon { get; init; }

    /// <summary>
    /// Gets the list of uncommon items that this Pokémon species may hold when encountered in the wild.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Drops")]
    public IReadOnlyList<FItemHandle> WildHoldItemUncommon { get; init; }

    /// <summary>
    /// Gets the list of rare items that this Pokémon species may hold when encountered in the wild.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Drops")]
    public IReadOnlyList<FItemHandle> WildHoldItemRare { get; init; }

    /// <summary>
    /// Gets the generation number in which this Pokémon species was introduced.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int Generation { get; init; } = 1;

    /// <summary>
    /// Gets the collection of gameplay tags associated with this Pokémon species.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    public FGameplayTagContainer Tags { get; init; }
}

/// <summary>
/// Provides a repository specifically for managing and accessing Pokémon species data.
/// This class extends functionality from the UAssetGameDataRepository, enabling structured
/// handling and queries of species-related game data.
/// </summary>
[UClass]
[GameDataRepository<FSpecies>]
public partial class USpeciesRepository : UAssetGameDataRepository;

/// <summary>
/// Represents a handle used to reference a Pokémon species, encapsulating a link
/// to its structure or metadata within the game data system. This type serves as
/// an identifier for species data and is utilized throughout various game systems
/// for operations or lookups involving Pokémon species.
/// </summary>
[DataHandle(typeof(GameData), nameof(GameData.Species))]
public readonly partial record struct FSpeciesHandle;
