using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Represents a berry plant configuration in the game, storing data related to plant growth,
/// identification, and yield properties.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FBerryPlant() : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the number of in-game hours required for a berry plant to transition through one growth stage.
    /// </summary>
    /// <remarks>
    /// This property is used for determining the growth progression rate of the berry plant.
    /// The value must be at least 1 to ensure proper growth mechanics in the game.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Growth")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int HoursPerStage { get; init; } = 3;

    /// <summary>
    /// Determines the rate at which the berry plant's soil dries out per hour.
    /// Measured in an integer value, it indicates the progressive depletion of water
    /// in the soil over time, affecting the plant's growth conditions.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Growth")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int DryingPerHour { get; init; } = 15;

    /// <summary>
    /// Gets or initializes the potential range of yield values for the berry plant.
    /// Represents the minimum and maximum number of berries that can be harvested
    /// from the plant during its lifecycle.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Growth")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    [AsValue]
    public FInt32Range Yield { get; init; }

    /// <summary>
    /// Gets the minimum yield value of the berry plant, as defined by the lower bound of the yield range.
    /// </summary>
    public int MinimumYield => Yield.LowerBound.Value;

    /// <summary>
    /// Gets the maximum possible yield value for the berry plant, corresponding to the upper bound of the yield range.
    /// </summary>
    public int MaximumYield => Yield.UpperBound.Value;
}

/// <summary>
/// Acts as a repository for managing and retrieving berry plant data within the game.
/// Responsible for providing access to instances of <see cref="FBerryPlant"/>
/// and interacting with associated game data mechanisms.
/// </summary>
[UClass]
[GameDataRepository<FBerryPlant>]
[UsedImplicitly]
public partial class UBerryPlantRepository : UAssetGameDataRepository;

/// <summary>
/// Represents a handle that uniquely identifies a berry plant entity within a game data repository.
/// Used to establish references or perform comparisons with other data entities like items.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BerryPlants), ComparableTypes = [typeof(FItemHandle)])]
public readonly partial record struct FBerryPlantHandle;
