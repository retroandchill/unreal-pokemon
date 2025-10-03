using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using LanguageExt;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Represents an ability within the game, containing its identification, display information, metadata, and tags.
/// </summary>
/// <remarks>
/// This struct is primarily used as part of the game's data infrastructure to model abilities,
/// providing a standardized structure for various characteristics such as name, description, and gameplay tags.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FAbility() : IGameDataEntry
{
    /// <inheritdoc />
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the display name of the ability.
    /// </summary>
    /// <remarks>
    /// This property represents the localized name of the ability as it appears in the game's UI.
    /// </remarks>
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    [DisplayName]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the description of the ability, typically providing details about its effects or purpose within the game.
    /// </summary>
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    public FText Description { get; init; } = "???";

    /// <summary>
    /// A collection of gameplay tags associated with the ability.
    /// </summary>
    /// <remarks>
    /// These tags are used to categorize abilities, represent gameplay mechanics, and facilitate interaction with other game systems.
    /// </remarks>
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    public FGameplayTagContainer Tags { get; init; }
}

/// <summary>
/// Represents a repository for managing and accessing data related to abilities within the game.
/// </summary>
/// <remarks>
/// The repository utilizes the underlying game data infrastructure to provide access and operations
/// for ability records, serving as an entry point for retrieving ability data.
/// This class extends the functionality of <c>UAssetGameDataRepository</c>, enabling interaction with
/// structured ability data through an associated data model.
/// </remarks>
[UClass]
[GameDataRepository<FAbility>]
[UsedImplicitly]
public partial class UAbilityRepository : UAssetGameDataRepository;

/// <summary>
/// Represents a handle or reference to an ability, used to interact with and refer to abilities within the game data system.
/// </summary>
/// <remarks>
/// This struct provides an abstraction for accessing specific abilities registered in the game's data, serving as a lightweight reference.
/// It is primarily used in components, data models, and gameplay systems to interact with abilities without duplicating the full ability details.
/// </remarks>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Abilities))]
public readonly partial record struct FAbilityHandle;
