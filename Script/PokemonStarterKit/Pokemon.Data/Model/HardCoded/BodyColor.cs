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
/// Represents the body color data entity with unique identification, row indexing,
/// and localized display name within the application.
/// </summary>
/// <remarks>
/// This struct is used within the game's data models to standardize and manage body color entries.
/// It implements the <see cref="IGameDataEntry"/> interface for compatibility with other
/// game data management systems.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FBodyColor : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets or sets the localized display name associated with the body color entry.
    /// </summary>
    /// <remarks>
    /// This property is used to represent the display name of the body color in a format
    /// that supports localization, allowing for user-friendly and language-specific naming within the game.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Provides access to and manages the repository for body color game data entries.
/// </summary>
/// <remarks>
/// This repository specializes in handling instances of <see cref="FBodyColor"/>, enabling
/// static access and data retrieval within the game's data layer. It inherits from
/// <see cref="UStaticGameDataRepository"/> to ensure compatibility with the static game
/// data infrastructure, offering efficient management of hard-coded body color entries.
/// </remarks>
[GameDataRepository<FBodyColor>]
public partial class UBodyColorRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to a body color entry within the game's static data.
/// </summary>
/// <remarks>
/// This record struct is used for referencing specific entries in the body color dataset.
/// It facilitates lookup and manipulation of body color data through the associated static repository.
/// </remarks>
[DataHandle(typeof(GameData), nameof(GameData.BodyColors))]
public readonly partial record struct FBodyColorHandle;

/// <summary>
/// Provides extension methods for managing body color entries within the game data manager.
/// </summary>
/// <remarks>
/// This static class defines helper methods to extend the functionality of <see cref="UGameDataManager"/> for registering
/// and managing instances of body color data. It simplifies the workflow for adding predefined body color entries to the
/// game's data repository.
/// </remarks>
public static class BodyColorExtensions
{
    private const string LocalizationNamespace = "GameData.BodyColor";

    /// <summary>
    /// Registers predefined body color entries into the game data manager.
    /// </summary>
    /// <param name="manager">The instance of <see cref="UGameDataManager"/> to which the body color entries will be added.</param>
    /// <returns>The updated instance of <see cref="UGameDataManager"/> with the body color entries registered.</returns>
    public static UGameDataManager AddBodyColors(this UGameDataManager manager)
    {
        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Red, Name = FText.Localized(LocalizationNamespace, "Red", "Red") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Blue, Name = FText.Localized(LocalizationNamespace, "Blue", "Blue") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Yellow, Name = FText.Localized(LocalizationNamespace, "Yellow", "Yellow") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Green, Name = FText.Localized(LocalizationNamespace, "Green", "Green") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Black, Name = FText.Localized(LocalizationNamespace, "Black", "Black") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Brown, Name = FText.Localized(LocalizationNamespace, "Brown", "Brown") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Purple, Name = FText.Localized(LocalizationNamespace, "Purple", "Purple") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Gray, Name = FText.Localized(LocalizationNamespace, "Gray", "Gray") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.White, Name = FText.Localized(LocalizationNamespace, "White", "White") }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor { ID = FBodyColor.Pink, Name = FText.Localized(LocalizationNamespace, "Pink", "Pink") }
        );

        return manager;
    }
}
