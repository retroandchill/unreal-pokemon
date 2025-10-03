using LanguageExt;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents the statistical component information of a Pokémon, including level, experience, health points,
/// individual values, effort values, and nature-related properties.
/// </summary>
[UStruct]
public readonly partial record struct FStatComponentInfo
{
    /// <summary>
    /// Gets or initializes the level of the Pokémon. Represents the numerical level
    /// that directly impacts a Pokémon's strength, growth, and evolution. This property
    /// is required for saving and loading the state of a Pokémon.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required int Level { get; init; }

    /// <summary>
    /// Gets or initializes the experience points of the Pokémon. Experience points represent
    /// the accumulated growth measure for the Pokémon, enabling leveling up and progression.
    /// This property is essential for tracking and saving the advancement of a Pokémon.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required int Exp { get; init; }

    /// <summary>
    /// Gets or initializes the health points (HP) of the Pokémon. Represents the current life value of the Pokémon,
    /// essential for determining its survivability and performance in battles. This property is preserved during
    /// save and load operations.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required int HP { get; init; }

    /// <summary>
    /// Represents a read-only collection of Individual Values (IV) for a Pokémon's main stats.
    /// IVs are unique to each Pokémon and indicate its innate potential for each statistic.
    /// This property plays a key role in determining the Pokémon's growth and strength and
    /// is essential for saving and loading the state of a Pokémon.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyDictionary<FMainStatHandle, int> IV { get; init; }

    /// <summary>
    /// Represents a collection of individual value (IV) overrides for a Pokémon's main stats.
    /// This property allows specific stat IVs to be manually overridden, disregarding the default/generated IV values.
    /// It plays a key role in customizing Pokémon stats while preserving the required structure for saving and loading.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyDictionary<FMainStatHandle, int> IVOverrides { get; init; }

    /// <summary>
    /// Gets or initializes the Effort Values (EVs) of the Pokémon. Represents the stat-specific
    /// points allocated to enhance the Pokémon's growth and performance in different attributes.
    /// This property is essential for saving and loading the Pokémon's customized training data.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyDictionary<FMainStatHandle, int> EV { get; init; }

    /// <summary>
    /// Gets or initializes the nature of the Pokémon. Determines the Pokémon's inherent
    /// statistical tendencies, influencing how certain stats are increased or decreased.
    /// This property is critical for calculating the Pokémon's performance and growth.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FNatureHandle Nature { get; init; }

    /// <summary>
    /// Gets or initializes an optional nature override for the Pokémon. This property allows setting
    /// a temporary or permanent override to the default nature assigned to the Pokémon, potentially
    /// altering its stat growth or behavior. If no override is present, the Pokémon uses its default nature.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required Option<FNatureHandle> NatureOverride { get; init; }
}
