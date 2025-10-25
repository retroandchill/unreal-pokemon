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
/// Represents the data structure for a body shape entry in the game.
/// This struct contains necessary properties to define a body shape,
/// including its unique identifier, row index, and localized name.
/// Implements <see cref="IGameDataEntry"/> to ensure consistency across game data models.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FBodyShape : IGameDataEntry
{
    /// <inheritdoc />
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the name of the body shape as a localized text.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Represents a repository for managing and accessing body shape data in the game.
/// This class serves as a specific implementation of <see cref="UStaticGameDataRepository" />
/// for handling body shape entries defined by <see cref="FBodyShape" />.
/// Intended for use in scenarios requiring organized, static access to body shape game data.
/// </summary>
[UClass]
[GameDataRepository<FBodyShape>]
[UsedImplicitly]
public partial class UBodyShapeRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to a specific body shape in the game data.
/// This struct serves as a lightweight reference or pointer to a body shape entry,
/// facilitating operations such as lookups or comparisons without directly copying
/// the associated data structure. The handle is designed to integrate with the game
/// data repository system, specifically targeting <see cref="GameData.BodyShapes"/>.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BodyShapes))]
public readonly partial record struct FBodyShapeHandle;

/// <summary>
/// Provides extension methods for integrating body shape data into the game data model.
/// This static class is responsible for adding predefined body shape entries to
/// an instance of <see cref="UGameDataManager"/>, enabling structured
/// management and registration of various body shape types.
/// </summary>
public static class BodyShapeExtensions
{
    private const string LocalizationNamespace = "GameData.BodyShape";

    /// <summary>
    /// Adds predefined body shapes to the <c>UGameDataManager</c> instance by registering various body shape entries.
    /// </summary>
    /// <param name="manager">The <c>UGameDataManager</c> instance to which the body shapes will be added.</param>
    /// <returns>The updated <c>UGameDataManager</c> instance after adding the body shapes.</returns>
    public static UGameDataManager AddBodyShapes(this UGameDataManager manager)
    {
        manager.BodyShapes.RegisterEntry(
            new FBodyShape { ID = FBodyShape.Head, Name = FText.Localized(LocalizationNamespace, "Head", "Head") }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Serpentine,
                Name = FText.Localized(LocalizationNamespace, "Serpentine", "Serpentine"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape { ID = FBodyShape.Finned, Name = FText.Localized(LocalizationNamespace, "Finned", "Finned") }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.HeadArms,
                Name = FText.Localized(LocalizationNamespace, "HeadArms", "Head and arms"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.HeadBase,
                Name = FText.Localized(LocalizationNamespace, "HeadBase", "Head and base"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.BipedalTail,
                Name = FText.Localized(LocalizationNamespace, "BipedalTail", "Bipedal with tail"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.HeadLegs,
                Name = FText.Localized(LocalizationNamespace, "HeadLegs", "Head and legs"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Quadruped,
                Name = FText.Localized(LocalizationNamespace, "Quadruped", "Quadruped"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape { ID = FBodyShape.Winged, Name = FText.Localized(LocalizationNamespace, "Winged", "Winged") }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Multiped,
                Name = FText.Localized(LocalizationNamespace, "Multiped", "Multiped"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.MultiBody,
                Name = FText.Localized(LocalizationNamespace, "MultiBody", "Multi Body"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Bipedal,
                Name = FText.Localized(LocalizationNamespace, "Bipedal", "Bipedal"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.MultiWinged,
                Name = FText.Localized(LocalizationNamespace, "MultiWinged", "Multi Winged"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Insectoid,
                Name = FText.Localized(LocalizationNamespace, "Insectoid", "Insectoid"),
            }
        );

        return manager;
    }
}
