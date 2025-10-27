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
/// Represents the type of egg group for breeding or classification purposes.
/// </summary>
[UEnum]
public enum EEggGroupType : byte
{
    /// <summary>
    /// Requires the other Pokémon to have the same egg group.
    /// </summary>
    WithSameEggGroup,

    /// <summary>
    /// Can breed with any egg group.
    /// </summary>
    WithAnyEggGroup,

    /// <summary>
    /// Cannot breed with any egg group.
    /// </summary>
    WithNoEggGroups,
}

/// <summary>
/// Represents a structured data model for an Egg Group in the game.
/// </summary>
/// <remarks>
/// This struct is utilized for managing information about different classifications
/// of Egg Groups, including their identifiers, display names, and specific breeding
/// properties categorized by the Egg Group type.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FEggGroup : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the name of the egg group.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Represents the breeding compatibility group type of a Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public EEggGroupType BreedingType { get; init; }
}

/// <summary>
/// Provides repository functionality for managing and accessing hard-coded data related to Egg Groups.
/// </summary>
/// <remarks>
/// This class serves as the data repository for <see cref="FEggGroup"/> entries, enabling retrieval and manipulation
/// of structured Egg Group metadata in the game. It extends the functionality of the base static game data
/// repository, offering specific support for handling the domain of Egg Groups.
/// </remarks>
[GameDataRepository<FEggGroup>]
public partial class UEggGroupRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a reference handle to an Egg Group data entry within the game.
/// </summary>
/// <remarks>
/// This struct serves as a lightweight reference to specific Egg Group data
/// managed by the game's dataset. It enables efficient access, lookup, and
/// usage of Egg Group records without directly handling the raw data.
/// </remarks>
[DataHandle(typeof(GameData), nameof(GameData.EggGroups))]
public readonly partial record struct FEggGroupHandle;

/// <summary>
/// Provides extension methods for managing and populating Egg Group-related data.
/// </summary>
/// <remarks>
/// This static class contains methods designed to extend the functionality of the
/// <c>UGameDataManager</c>, enabling streamlined handling and registration of Egg Groups
/// into the game's data model. The class offers tools to define, classify, and organize
/// Egg Groups used within the application.
/// </remarks>
public static class EggGroupExtensions
{
    private const string LocalizationNamespace = "GameData.EggGroup";

    /// <summary>
    /// Adds predefined egg groups to the given game data manager instance.
    /// This method registers multiple default egg groups into the provided game data manager.
    /// These egg groups are used to categorize Pokémon breeding types and their compatibility.
    /// </summary>
    /// <param name="manager">The instance of <see cref="UGameDataManager"/> to which the egg groups will be added.</param>
    /// <returns>The updated <see cref="UGameDataManager"/> instance with the registered egg groups.</returns>
    public static UGameDataManager AddEggGroups(this UGameDataManager manager)
    {
        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Undiscovered,
                Name = FText.Localized(LocalizationNamespace, "Undiscovered", "Undiscovered"),
                BreedingType = EEggGroupType.WithNoEggGroups,
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Monster,
                Name = FText.Localized(LocalizationNamespace, "Monster", "Monster"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Water1, Name = FText.Localized(LocalizationNamespace, "Water1", "Water 1") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Bug, Name = FText.Localized(LocalizationNamespace, "Bug", "Bug") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Flying, Name = FText.Localized(LocalizationNamespace, "Flying", "Flying") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Field, Name = FText.Localized(LocalizationNamespace, "Field", "Field") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Fairy, Name = FText.Localized(LocalizationNamespace, "Fairy", "Fairy") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Grass, Name = FText.Localized(LocalizationNamespace, "Grass", "Grass") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Humanlike,
                Name = FText.Localized(LocalizationNamespace, "Humanlike", "Humanlike"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Water3, Name = FText.Localized(LocalizationNamespace, "Water3", "Water 3") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Mineral,
                Name = FText.Localized(LocalizationNamespace, "Mineral", "Mineral"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Amorphous,
                Name = FText.Localized(LocalizationNamespace, "Amorphous", "Amorphous"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Water2, Name = FText.Localized(LocalizationNamespace, "Water2", "Water 2") }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Ditto,
                Name = FText.Localized(LocalizationNamespace, "Ditto", "Ditto"),
                BreedingType = EEggGroupType.WithAnyEggGroup,
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup { ID = FEggGroup.Dragon, Name = FText.Localized(LocalizationNamespace, "Dragon", "Dragon") }
        );

        return manager;
    }
}
