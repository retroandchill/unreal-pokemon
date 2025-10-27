using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents a habitat in the game, containing details such as its unique identifier,
/// row index, and display name.
/// This struct is primarily used for managing and accessing habitat-related data
/// within the game.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FHabitat : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the display name of the habitat. This property is localized to support
    /// multiple languages and is used for rendering or displaying the name in the game.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Provides a repository for managing and accessing habitat-related game data.
/// This class serves as a highly specific implementation of a static game data repository,
/// tailored to handle entries of type <see cref="FHabitat"/>.
/// </summary>
[GameDataRepository<FHabitat>]
public partial class UHabitatRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to a habitat within the game. This struct acts as a lightweight
/// identifier or reference for a specific habitat, enabling efficient access and management
/// of habitat-related data.
/// </summary>
[DataHandle(typeof(GameData), nameof(GameData.Habitats))]
public readonly partial record struct FHabitatHandle;

/// <summary>
/// Provides extension methods for managing and registering habitat-related data
/// within the game. This class serves as a utility to simplify configuration
/// and initialization tasks associated with habitats.
/// </summary>
public static class HabitatExtensions
{
    private const string LocalizationNamespace = "GameData.Habitat";

    /// <summary>
    /// Adds predefined habitat entries to the game data manager.
    /// </summary>
    /// <param name="manager">The instance of <see cref="UGameDataManager"/> to which habitat entries should be added.</param>
    /// <returns>The <see cref="UGameDataManager"/> instance with the added habitat entries.</returns>
    public static UGameDataManager AddHabitats(this UGameDataManager manager)
    {
        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Grassland,
                Name = FText.Localized(LocalizationNamespace, "Grassland", "Grassland"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat { ID = FHabitat.Forest, Name = FText.Localized(LocalizationNamespace, "Forest", "Forest") }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.WatersEdge,
                Name = FText.Localized(LocalizationNamespace, "WatersEdge", "Water's Edge"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat { ID = FHabitat.Sea, Name = FText.Localized(LocalizationNamespace, "Sea", "Sea") }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat { ID = FHabitat.Cave, Name = FText.Localized(LocalizationNamespace, "Cave", "Cave") }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Mountain,
                Name = FText.Localized(LocalizationNamespace, "Mountain", "Mountain"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.RoughTerrain,
                Name = FText.Localized(LocalizationNamespace, "RoughTerrain", "Rough Terrain"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat { ID = FHabitat.Urban, Name = FText.Localized(LocalizationNamespace, "Urban", "Urban") }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat { ID = FHabitat.Rare, Name = FText.Localized(LocalizationNamespace, "Rare", "Rare") }
        );

        return manager;
    }
}
