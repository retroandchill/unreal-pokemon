using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using Pokemon.Editor.Serialization.Pbs.Converters;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents an EV (Effort Value) yield for a specific in-game stat.
/// Each EV yield consists of a stat and an associated amount that is gained
/// upon defeating a certain Pokémon in the game.
/// </summary>
/// <remarks>
/// EV yields are used to enhance specific stats for a Pokémon during its growth
/// and are part of the broader Pokémon training mechanics. Each yield specifies
/// which stat is affected and how much is gained.
/// </remarks>
/// <param name="Stat">
/// The name of the stat affected by the EV yield. For example, stats can include
/// Attack, Defense, Speed, etc.
/// </param>
/// <param name="Amount">
/// The number of EV points gained for the specified stat. This value must be at least 1.
/// </param>
public readonly record struct EvYield(FName Stat, [PbsRange<int>(1)] int Amount);

/// <summary>
/// Represents the association of a Pokémon move with a level at which it can be learned.
/// </summary>
/// <remarks>
/// This struct is used to define the level-up learnset of a Pokémon species. Each entry specifies
/// a move and the level at which the Pokémon learns that move as part of its progression.
/// A level of 0 denotes that the move is learned upon evolution rather than through leveling up.
/// </remarks>
/// <param name="Level">
/// The level at which the move is learned. A value of 0 indicates the move is learned
/// due to evolution rather than leveling up. The minimum value is 0.
/// </param>
/// <param name="Move">
/// The identifier for the move being learned. This is used to link the information to
/// the corresponding move data.
/// </param>
public record struct LevelUpMoveInfo([PbsRange<int>(0)] int Level, FName Move);

/// <summary>
/// Represents the conditions required for a Pokémon to evolve into a specified species.
/// This includes the species to evolve into, the method of evolution, and any additional
/// parameters required to fulfill the evolution condition.
/// </summary>
/// <remarks>
/// This structure is used to define the evolution mechanics for a Pokémon, detailing
/// both the method and any accompanying conditions that must be met. Examples of evolution
/// methods could include leveling up, using an item, trading, or meeting specific criteria.
/// </remarks>
/// <param name="Species">
/// The target species that the Pokémon evolves into. Represents a unique identifier for the species.
/// </param>
/// <param name="Method">
/// The method by which the evolution is triggered. Examples include "LevelUp" or "ItemUsage",
/// which specify the required action or event to evolve.
/// </param>
/// <param name="Parameters">
/// Additional parameters that may be necessary to fulfill the evolution condition. If no extra
/// parameters are required, a default value is used.
/// </param>
[PbsScalar<EvolutionMethodConverter>]
public record struct EvolutionConditionInfo(FName Species, FName Method, FInstancedStruct Parameters = default);

/// <summary>
/// Represents detailed information about a Pokémon species, including its attributes and relevant gameplay data.
/// </summary>
/// <remarks>
/// This class serves as a data structure to encapsulate various properties of a Pokémon species used in the game.
/// It includes basic identifiers, statistical attributes, abilities, moves, evolution conditions, and other
/// biological and gameplay-related data.
/// </remarks>
public record SpeciesInfo
{
    /// <summary>
    /// Represents the unique identifier for a species information record.
    /// This property is marked with the PbsKey attribute to denote its significance as a primary key in the PBS data structure.
    /// </summary>
    [PbsKey]
    public FName ID { get; init; }

    /// <summary>
    /// Represents the index of the row in the PBS data structure for species information.
    /// This property is marked with the PbsIndex attribute, indicating its role for ordered or indexed data storage.
    /// </summary>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the display name of a species in the Pokémon database.
    /// This property is localized using the PbsLocalizedText attribute
    /// to allow for dynamic text generation based on the species namespace and key format.
    /// </summary>
    [PbsLocalizedText("PokemonSpecies", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Represents the localized text for the specific form name of a Pokémon species.
    /// This property is annotated with the PbsLocalizedText attribute, which assists in retrieving
    /// the appropriate localized string by utilizing the given namespace and key format.
    /// </summary>
    [PbsLocalizedText("PokemonSpecies", "{0}_FormName")]
    public FText FormName { get; init; } = FText.None;

    /// <summary>
    /// Represents the list of types associated with a species.
    /// This property includes a PbsLength attribute to enforce a minimum required length of at least one type in the list.
    /// </summary>
    [PbsLength(Min = 1)]
    public IReadOnlyList<FName> Types { get; init; } = [new("NORMAL")];

    /// <summary>
    /// Represents the base stats for a species in the Pokémon data model.
    /// This property is a read-only dictionary mapping stat categories (e.g., HP, Attack, Defense, etc.)
    /// to their respective values. It is decorated with the PbsScalar attribute and utilizes the
    /// BaseStatsConverter for serialization and deserialization.
    /// </summary>
    [PbsScalar<BaseStatsConverter>]
    public IReadOnlyDictionary<FName, int> BaseStats { get; init; }

    /// <summary>
    /// Defines the gender ratio for a species.
    /// This property determines the distribution of male and female entities for the given species.
    /// </summary>
    public FName GenderRatio { get; init; } = FGenderRatio.Female50Percent;

    /// <summary>
    /// Specifies the growth rate associated with a species, determining the experience curve required for the species to level up.
    /// This property is initialized with a default value of `FGrowthRate.Medium`.
    /// </summary>
    public FName GrowthRate { get; init; } = FGrowthRate.Medium;

    /// <summary>
    /// Represents the base experience yield of the species.
    /// This property determines the amount of experience points gained by defeating the species in battle.
    /// </summary>
    public int BaseExp { get; init; } = 100;

    /// <summary>
    /// Represents the Effort Value (EV) distribution associated with a species.
    /// This property consists of a collection of EVYield records, which define the stats
    /// and the amount of EV points a species grants when defeated.
    /// It is annotated with the PbsName attribute for mapping purposes in the PBS data structure.
    /// </summary>
    [PbsName("EVs")]
    public IReadOnlyList<EvYield> EVYield { get; init; } = [];

    /// <summary>
    /// Represents the capture rate of the species, indicating the likelihood of successfully catching it in the game.
    /// The value is restricted by the PbsRange attribute to be between 3 and 255.
    /// </summary>
    [PbsRange<int>(3, 255)]
    public int CatchRate { get; init; } = 255;

    /// <summary>
    /// Defines the initial level of happiness for a species, ranging from 0 to 255.
    /// This property is annotated with the PbsRange attribute to enforce the valid range constraints.
    /// </summary>
    [PbsRange<int>(0, 255)]
    public int BaseHappiness { get; init; } = 70;

    /// <summary>
    /// Represents a collection of abilities associated with a species.
    /// This property is restricted in length, with a maximum of two entries, as marked by the PbsLength attribute.
    /// </summary>
    [PbsLength(Max = 2)]
    public IReadOnlyList<FName> Abilities { get; init; } = [];

    /// <summary>
    /// Represents a collection of hidden abilities associated with a species.
    /// These are special abilities that a species can possess, which differ
    /// from their standard abilities and may only be accessible under specific conditions.
    /// </summary>
    public IReadOnlyList<FName> HiddenAbilities { get; init; } = [];

    /// <summary>
    /// Represents the collection of level-up moves associated with a species.
    /// This property holds a read-only list of <see cref="LevelUpMoveInfo"/> objects,
    /// defining the moves learned by the species at specific levels.
    /// </summary>
    public IReadOnlyList<LevelUpMoveInfo> Moves { get; init; } = [];

    /// <summary>
    /// Represents the list of moves that can be taught to the species by a tutor.
    /// This property is read-only and contains the moves as a collection of names.
    /// </summary>
    public IReadOnlyList<FName> TutorMoves { get; init; } = [];

    /// <summary>
    /// Contains a read-only list of egg moves associated with a Pokémon species.
    /// Egg moves represent special moves that a Pokémon can inherit through breeding.
    /// </summary>
    public IReadOnlyList<FName> EggMoves { get; init; } = [];

    /// <summary>
    /// Represents the collection of egg groups applicable to a specific Pokémon species.
    /// This property is designed as a read-only list of FEggGroup identifiers,
    /// indicating compatibility for breeding or other relevant classifications in Pokémon taxonomy.
    /// </summary>
    public IReadOnlyList<FName> EggGroups { get; init; } = [FEggGroup.Undiscovered];

    /// <summary>
    /// Indicates the number of steps required to hatch a Pokémon egg for the species represented by this record.
    /// This property plays a critical role in determining the effort needed for the hatching process in the game mechanics.
    /// </summary>
    public int HatchSteps { get; init; } = 1;

    /// <summary>
    /// Represents the associated incense for a species, which is likely used as part of a gameplay
    /// mechanic or to define specific breeding or item-related behaviors in the dataset.
    /// </summary>
    public FName Incense { get; init; }

    /// <summary>
    /// Represents a collection of offspring identifiers associated with the species.
    /// This property contains a read-only list of names that define the offspring related to the species in the species information record.
    /// </summary>
    public IReadOnlyList<FName> Offspring { get; init; } = [];

    /// <summary>
    /// Represents a collection of evolution conditions for a Pokémon species.
    /// Each evolution condition specifies the target species, the method by which the evolution occurs,
    /// and any additional parameters required for the evolution to take place.
    /// This property allows multiple entries, enabling a species to have multiple evolutionary paths.
    /// </summary>
    [PbsAllowMultiple]
    [PbsName("Evolution")]
    public IReadOnlyList<EvolutionConditionInfo> Evolutions { get; init; } = [];

    /// <summary>
    /// Represents the height of a species, measured in meters.
    /// This property is constrained by the PbsRange attribute, ensuring it cannot fall below the specified minimum value.
    /// </summary>
    [PbsRange<float>(0.1f)]
    public float Height { get; init; } = 0.1f;

    /// <summary>
    /// Represents the weight of a species in the PBS data structure.
    /// This property is constrained by the PbsRange attribute, which enforces a minimum allowable value of 0.1.
    /// </summary>
    [PbsRange<float>(0.1f)]
    public float Weight { get; init; } = 0.1f;

    /// <summary>
    /// Represents the body color of a species in the Pokémon data model.
    /// This property is associated with the "Color" field in the PBS data structure and utilizes the FBodyColor struct to define its value.
    /// </summary>
    [PbsName("Color")]
    public FName BodyColor { get; init; } = FBodyColor.Red;

    /// <summary>
    /// Defines the physical body shape of a species, represented using the FBodyShape type.
    /// This property is associated with the "Shape" identifier in the PBS data structure through the PbsName attribute.
    /// </summary>
    [PbsName("Shape")]
    public FName BodyShape { get; init; } = FBodyShape.Head;

    /// <summary>
    /// Denotes the natural environment or ecological area where a species is typically found.
    /// This property provides insight into the species' dwelling characteristics within the dataset.
    /// </summary>
    public FName Habitat { get; init; }

    /// <summary>
    /// Represents the localized text for the category of a Pokémon species.
    /// This property is decorated with the PbsLocalizedText attribute to specify the namespace
    /// and key format used for localization within the Pokémon species data.
    /// </summary>
    [PbsLocalizedText("PokemonSpecies", "{0}_Category")]
    public FText Category { get; init; } = "???";

    /// <summary>
    /// Represents the localized text for the Pokedex entry of a Pokémon species.
    /// This property is annotated with the PbsLocalizedText attribute to define its localization namespace
    /// and key format for retrieving the appropriate text.
    /// </summary>
    [PbsLocalizedText("PokemonSpecies", "{0}_Pokedex")]
    public FText Pokedex { get; init; } = "???";

    /// <summary>
    /// Represents a collection of items commonly held by wild Pokémon of the species.
    /// This property provides a list of item identifiers, reflecting the potential items
    /// that may appear when a wild Pokémon is encountered.
    /// </summary>
    public IReadOnlyList<FName> WildHoldItemCommon { get; init; } = [];

    /// <summary>
    /// Represents a collection of items that a wild Pokémon may commonly hold.
    /// This property contains an immutable list of item identifiers defining the uncommon held items for a species.
    /// </summary>
    public IReadOnlyList<FName> WildHoldItemUncommon { get; init; } = [];

    /// <summary>
    /// Specifies a collection of rare items that a wild Pokémon species may hold.
    /// This property is used to define uncommon item drops associated with the species.
    /// </summary>
    public IReadOnlyList<FName> WildHoldItemRare { get; init; } = [];

    /// <summary>
    /// Defines the generation to which a species belongs.
    /// This property is constrained to a minimum value of 1 using the PbsRange attribute, ensuring valid generation values.
    /// </summary>
    [PbsRange<int>(1)]
    public int Generation { get; init; } = 1;

    /// <summary>
    /// Represents a container of gameplay tags associated with a species.
    /// This property is annotated with the PbsName and PbsGameplayTag attributes,
    /// which facilitate its usage in the PBS data structure with specific serialization behavior and namespace configurations.
    /// </summary>
    [PbsName("Flags")]
    [PbsGameplayTag("Data.Species", Create = true, Separator = "_")]
    public FGameplayTagContainer Tags { get; init; }

    /// <summary>
    /// Represents information about a Pokémon species, including its base stats.
    /// </summary>
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
