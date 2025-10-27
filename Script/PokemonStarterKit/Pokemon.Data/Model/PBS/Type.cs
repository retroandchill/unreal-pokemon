using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Represents a type definition used in the game data model.
/// </summary>
/// <remarks>
/// This structure encapsulates the attributes and relationships of a game type, including its
/// identification, display information, physical and special type properties, and type interactions
/// such as weaknesses, resistances, and immunities. This struct is integral in defining type-specific
/// gameplay mechanics and metadata.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FType() : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the display name of the Pokémon type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    [DisplayName]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets a value indicating whether this type is classified as a Physical type.
    /// Physical types typically correspond to moves that make direct contact.
    /// </summary>
    public bool IsPhysicalType => !IsSpecialType;

    /// <summary>
    /// Gets a value indicating whether this type is classified as a Special type.
    /// Special types typically correspond to moves that deal damage from a distance.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Type Info")]
    public bool IsSpecialType { get; init; } = false;

    /// <summary>
    /// Gets a value indicating whether this type is considered a pseudo-type.
    /// Pseudo-types are special classifications that don't function as regular types
    /// but may have special meaning in certain game mechanics.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Type Info")]
    public bool IsPseudoType { get; init; } = false;

    /// <summary>
    /// Gets the list of types that this type is weak against.
    /// Attacks of these types will deal double damage to Pokémon of this type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Type Info")]
    public IReadOnlyList<FTypeHandle> Weaknesses { get; init; } = [];

    /// <summary>
    /// Gets the list of types that this type is resistant to.
    /// Attacks of these types will deal half damage to Pokémon of this type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Type Info")]
    public IReadOnlyList<FTypeHandle> Resistances { get; init; } = [];

    /// <summary>
    /// Gets the list of types that this type is immune to.
    /// Attacks of these types will deal no damage to Pokémon of this type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Type Info")]
    public IReadOnlyList<FTypeHandle> Immunities { get; init; } = [];

    /// <summary>
    /// Gets the collection of gameplay tags associated with this type.
    /// These tags can be used for various gameplay mechanics and type-specific effects.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    public FGameplayTagContainer Tags { get; init; }
}

/// <summary>
/// Represents the repository for managing FType game data entries in the application.
/// </summary>
/// <remarks>
/// This class is responsible for handling the retrieval and management of FType entities,
/// derived from the UAssetGameDataRepository base class. It facilitates operations
/// on type-related game data, including structure definition and weak type handling.
/// </remarks>
[UClass]
[GameDataRepository<FType>]
public partial class UTypeRepository : UAssetGameDataRepository;

/// <summary>
/// Represents a handle for accessing and identifying FType game data within the system.
/// </summary>
/// <remarks>
/// This record struct facilitates efficient referencing of type-related game data entries,
/// serving as a lightweight identifier. It is primarily used in association with other
/// game components and repositories, where it acts as a key to retrieve or operate on
/// specific type information.
/// </remarks>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Types))]
public readonly partial record struct FTypeHandle([property: UProperty(PropertyFlags.EditAnywhere)] FName ID);
