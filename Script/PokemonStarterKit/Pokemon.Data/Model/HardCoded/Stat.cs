using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;
using ZLinq;
using ZLinq.Linq;

namespace Pokemon.Data.Model.HardCoded;

using FilteredStats = ValueEnumerable<
    WhereSelect<
        StructArrayValueEnumerator<FStat, (FName ID, FText Name, EStatType StatType)>,
        (FName ID, FText Name, EStatType StatType),
        FDataHandleEntry
    >,
    FDataHandleEntry
>;

/// <summary>
/// Represents the types of statistics associated with an entity in the system.
/// </summary>
/// <remarks>
/// This enumeration defines the different categories of stats that can be used
/// to classify and manage various attributes related to gameplay or data structures.
/// </remarks>
[UEnum]
public enum EStatType : byte
{
    /// <summary>
    /// Main stat, can't be raised/lowered in battle.
    /// </summary>
    Main,

    /// <summary>
    /// Main stat, can be raised/lowered in battle.
    /// </summary>
    MainBattle,

    /// <summary>
    /// Can be raised/lowered in battle, not part of the Pokémon's stats.
    /// </summary>
    Battle,
}

/// <summary>
/// Represents statistical data used in the game. This structure provides detailed attributes of a specific stat, including its identifier, name, type, and additional metadata.
/// </summary>
/// <remarks>
/// This struct is useful for managing and categorizing game statistics, particularly for those related to primary and battle-centric operations within the game logic.
/// Instances of this structure are typically defined and manipulated to configure or retrieve game-related stat details.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FStat : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the name of the stat as a localized text.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Represents a brief display name or shorthand identifier for the stat.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FText NameBrief { get; init; }

    /// <summary>
    /// Represents the type of a stat, which determines its category or purpose within the game context.
    /// </summary>
    /// <remarks>
    /// The <see cref="StatType"/> can indicate whether a stat is primarily for main gameplay, battle scenarios, or both.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required EStatType StatType { get; init; }

    /// <summary>
    /// Gets the order of the stat as defined in the Pokémon Battle System (PBS).
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, DisplayName = "PBS Order")]
    public int PBSOrder { get; init; }

    /// <summary>
    /// Indicates whether the stat is a main statistical category.
    /// </summary>
    /// <remarks>
    /// A stat is considered a main stat if its type is either <c>EStatType.Main</c>
    /// or <c>EStatType.MainBattle</c>. This property is useful for filtering and classifying
    /// statistics that are central or primary within the game.
    /// </remarks>
    public bool IsMainStat => StatType is EStatType.Main or EStatType.MainBattle;

    /// <summary>
    /// Indicates whether the current stat is associated with battle-specific operations or configurations.
    /// </summary>
    /// <remarks>
    /// This property evaluates the <see cref="StatType"/> to determine if it matches
    /// <see cref="EStatType.Battle"/> or <see cref="EStatType.MainBattle"/>.
    /// </remarks>
    public bool IsBattleStat => StatType is EStatType.Battle or EStatType.MainBattle;
}

/// <summary>
/// Represents a repository for managing static game data related to statistical attributes.
/// </summary>
/// <remarks>
/// This class acts as a specialized repository for handling instances of <see cref="FStat"/>,
/// which encapsulate specific statistical properties used within the game logic.
/// The repository leverages the capabilities of its base class to enable efficient
/// retrieval and management of static statistical data.
/// </remarks>
[GameDataRepository<FStat>]
public partial class UStatRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a lightweight, read-only handle for accessing statistical data within the game.
/// </summary>
/// <remarks>
/// This struct enables efficient management and retrieval of specific game stats by providing a reference-like behavior to the underlying statistical data.
/// Typically used in operations where only referencing of data is needed without requiring the full data footprint or direct modification.
/// It is compatible with various comparable types such as FMainStatHandle, FBattleStatHandle, and FMainBattleStatHandle for flexible data handling.
/// </remarks>
[DataHandle(
    typeof(GameData),
    nameof(GameData.Stats),
    ComparableTypes = [typeof(FMainStatHandle), typeof(FBattleStatHandle), typeof(FMainBattleStatHandle)]
)]
public readonly partial record struct FStatHandle;

/// <summary>
/// Represents a handle to a primary game statistic. This handle references a statistical entry that falls under the main or main battle categories within the game logic.
/// </summary>
/// <remarks>
/// The structure is primarily used for identifying and working with main statistics in the game's data set. It ensures that only valid main or main battle statistics are accessed or manipulated.
/// The handle facilitates efficient querying, validation, and retrieval of associated main statistic entries.
/// </remarks>
[DataHandle<FStat>(ComparableTypes = [typeof(FStatHandle), typeof(FBattleStatHandle), typeof(FMainBattleStatHandle)])]
public readonly partial record struct FMainStatHandle
{
    /// <summary>
    /// Provides filtered access to a collection of main and main battle stat entries from game data,
    /// returning them as a sequence of data handle entries.
    /// </summary>
    public static FilteredStats Entries
    {
        get
        {
            return GameData
                .Stats.Entries.AsValueEnumerable(s => (s.ID, s.Name, s.StatType))
                .Where(s => s.StatType is EStatType.Main or EStatType.MainBattle)
                .Select(s => new FDataHandleEntry(s.ID, s.Name));
        }
    }

    static IEnumerable<FDataHandleEntry> IDataHandle.Entries => Entries.ToArray();

    /// <inheritdoc />
    public bool IsValid
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry))
            {
                return false;
            }

            return entry.StatType is EStatType.Main or EStatType.MainBattle;
        }
    }

    /// <inheritdoc />
    public StructView<FStat> Entry
    {
        get
        {
            if (
                !GameData.Stats.TryGetEntry(ID, out var entry)
                || entry.StatType is not EStatType.Main and not EStatType.MainBattle
            )
            {
                throw new InvalidOperationException("Invalid stat handle!");
            }

            return entry;
        }
    }
}

/// <summary>
/// Represents a handle for a battle-related statistic in the game. This record struct serves as a reference to specific statistics categorized as battle or main battle types.
/// </summary>
/// <remarks>
/// The <see cref="FBattleStatHandle"/> provides an interface for accessing and validating game statistics associated with battle-related operations. It helps streamline the process of querying and retrieving structured data for relevant stats.
/// It is primarily suited to work with game data entries tied to the `FStat` struct, focusing on those with stat types classified under `EStatType.Battle` or `EStatType.MainBattle`.
/// </remarks>
[DataHandle<FStat>(ComparableTypes = [typeof(FStatHandle), typeof(FMainStatHandle), typeof(FMainBattleStatHandle)])]
public readonly partial record struct FBattleStatHandle
{
    /// <summary>
    /// Gets a filtered enumeration of battle-related stat entries.
    /// </summary>
    /// <remarks>
    /// This property retrieves an enumerable collection of filtered statistics, extracted from the game's stat data and focused on the entries classified as having a stat type
    /// of either <see cref="EStatType.Battle"/> or <see cref="EStatType.MainBattle"/>. The resulting collection provides structured access to these specific statistical entries for use in relevant operations or validations.
    /// </remarks>
    public static FilteredStats Entries
    {
        get
        {
            return GameData
                .Stats.Entries.AsValueEnumerable(s => (s.ID, s.Name, s.StatType))
                .Where(s => s.StatType is EStatType.Battle or EStatType.MainBattle)
                .Select(s => new FDataHandleEntry(s.ID, s.Name));
        }
    }

    static IEnumerable<FDataHandleEntry> IDataHandle.Entries => Entries.ToArray();

    /// <inheritdoc />
    public bool IsValid
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry))
            {
                return false;
            }

            return entry.StatType is EStatType.Battle or EStatType.MainBattle;
        }
    }

    /// <inheritdoc />
    public StructView<FStat> Entry
    {
        get
        {
            if (
                !GameData.Stats.TryGetEntry(ID, out var entry)
                || entry.StatType is not EStatType.Battle and not EStatType.MainBattle
            )
            {
                throw new InvalidOperationException("Invalid stat handle!");
            }

            return entry;
        }
    }
}

/// <summary>
/// Represents a handle for accessing and managing main battle-specific statistics within the game.
/// This structure provides functionality to query, validate, and retrieve details pertaining to stats
/// categorized under the "Main Battle" type.
/// </summary>
/// <remarks>
/// This handle is designed for managing "Main Battle" statistics from the game's data repository.
/// It ensures that only statistics defined as "Main Battle" are accessed or manipulated.
/// This structure allows for efficient filtering and validation of specific stat types through its properties
/// and associated methods, promoting structured access and data integrity.
/// </remarks>
[DataHandle<FStat>(ComparableTypes = [typeof(FStatHandle), typeof(FBattleStatHandle), typeof(FMainStatHandle)])]
public readonly partial record struct FMainBattleStatHandle
{
    /// <summary>
    /// Represents a filtered collection of entries containing main battle statistics.
    /// </summary>
    /// <remarks>
    /// This property provides access to a collection of structured data entries,
    /// specifically those that are categorized under the "Main Battle" type. It is
    /// designed for efficient retrieval and processing of relevant game statistics
    /// with predefined filters applied. The entries are represented as enumerable
    /// data handles, allowing for iteration and additional query operations.
    /// </remarks>
    public static FilteredStats Entries
    {
        get
        {
            return GameData
                .Stats.Entries.AsValueEnumerable(s => (s.ID, s.Name, s.StatType))
                .Where(s => s.StatType == EStatType.MainBattle)
                .Select(s => new FDataHandleEntry(s.ID, s.Name));
        }
    }

    static IEnumerable<FDataHandleEntry> IDataHandle.Entries => Entries.ToArray();

    /// <inheritdoc />
    public bool IsValid
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry))
            {
                return false;
            }

            return entry.StatType == EStatType.MainBattle;
        }
    }

    /// <inheritdoc />
    public StructView<FStat> Entry
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry) || entry.StatType != EStatType.MainBattle)
            {
                throw new InvalidOperationException("Invalid stat handle!");
            }

            return entry;
        }
    }
}

/// <summary>
/// Provides extension methods for managing and manipulating game statistics within the game's data manager.
/// </summary>
/// <remarks>
/// This static class extends the functionality of the UGameDataManager by incorporating methods related to the handling of statistical entries.
/// It facilitates the streamlined registration and setup of predefined stats, making it easier to organize and update game statistics.
/// </remarks>
public static class StatExtensions
{
    private const string LocalizationNamespace = "GameData.Stat";

    /// <summary>
    /// Adds stat entries including main stats, battle stats, and other relevant stats to the game data manager.
    /// </summary>
    /// <param name="manager">The game data manager instance to which the stats will be added.</param>
    /// <returns>The updated instance of the game data manager containing the registered stats.</returns>
    public static UGameDataManager AddStats(this UGameDataManager manager)
    {
        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.HP,
                Name = FText.Localized(LocalizationNamespace, "HP", "HP"),
                NameBrief = FText.Localized(LocalizationNamespace, "HP", "HP"),
                StatType = EStatType.Main,
                PBSOrder = 0,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.ATTACK,
                Name = FText.Localized(LocalizationNamespace, "Attack", "Attack"),
                NameBrief = FText.Localized(LocalizationNamespace, "Atk", "Atk"),
                StatType = EStatType.MainBattle,
                PBSOrder = 1,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.DEFENSE,
                Name = FText.Localized(LocalizationNamespace, "Defense", "Defense"),
                NameBrief = FText.Localized(LocalizationNamespace, "Def", "Def"),
                StatType = EStatType.MainBattle,
                PBSOrder = 2,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.SPECIAL_ATTACK,
                Name = FText.Localized(LocalizationNamespace, "SpecialAttack", "SpecialAttack"),
                NameBrief = FText.Localized(LocalizationNamespace, "SpAtk", "SpAtk"),
                StatType = EStatType.MainBattle,
                PBSOrder = 4,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.SPECIAL_DEFENSE,
                Name = FText.Localized(LocalizationNamespace, "SpecialDefense", "SpecialDefense"),
                NameBrief = FText.Localized(LocalizationNamespace, "SpDef", "SpDef"),
                StatType = EStatType.MainBattle,
                PBSOrder = 5,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.SPEED,
                Name = FText.Localized(LocalizationNamespace, "Speed", "Speed"),
                NameBrief = FText.Localized(LocalizationNamespace, "Spd", "Spd"),
                StatType = EStatType.MainBattle,
                PBSOrder = 3,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.ACCURACY,
                Name = FText.Localized(LocalizationNamespace, "accuracy", "accuracy"),
                NameBrief = FText.Localized(LocalizationNamespace, "Acc", "Acc"),
                StatType = EStatType.Battle,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.EVASION,
                Name = FText.Localized(LocalizationNamespace, "evasiveness", "evasiveness"),
                NameBrief = FText.Localized(LocalizationNamespace, "Eva", "Eva"),
                StatType = EStatType.Battle,
            }
        );

        return manager;
    }
}
