using Pokemon.Data;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents metadata and characteristics of a move in the Pokémon game.
/// </summary>
public record MoveInfo
{
    /// <summary>
    /// Gets the unique identifier for the move.
    /// </summary>
    /// <remarks>
    /// This property represents a unique key associated with a move in the Pokémon data model.
    /// It is primarily used to identify and differentiate moves in the system.
    /// </remarks>
    [PbsKey]
    public FName ID { get; init; }

    /// <summary>
    /// Gets the row index of the move entry within the PBS file.
    /// </summary>
    /// <remarks>
    /// This property indicates the sequential position of the move's data row
    /// in the associated Pokémon Script file, allowing for efficient indexing
    /// and organization of move data.
    /// </remarks>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the localized display name of the move.
    /// </summary>
    /// <remarks>
    /// This property retrieves the textual representation of the move's name, which is localized based on
    /// the specified key format and namespace. It is primarily used to display the move's name in user interfaces
    /// or other contextual outputs. If no localized name is provided, a default value of "Unnamed" is used.
    /// </remarks>
    [PbsLocalizedText("PokemonMoves", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the localized description of the move.
    /// </summary>
    /// <remarks>
    /// This property provides a textual description of the move that is localized
    /// for different regions or languages. It is used to give players information
    /// about the move, such as its effects or characteristics.
    /// </remarks>
    [PbsLocalizedText("PokemonMoves", "{0}_Description")]
    public FText Description { get; init; } = "???";

    /// <summary>
    /// Gets the elemental or typing associated with the move.
    /// </summary>
    /// <remarks>
    /// This property indicates the type of the Pokémon move, such as Fire, Water, Grass, etc.
    /// It determines type effectiveness and interaction with other moves and Pokémon typings in the battle system.
    /// </remarks>
    public FName Type { get; init; }

    /// <summary>
    /// Gets the damage category of the move.
    /// </summary>
    /// <remarks>
    /// This property specifies the type of damage the move inflicts, such as Physical, Special, or Status.
    /// It helps determine how the move interacts with the target's stats and resistances during gameplay.
    /// </remarks>
    public EDamageCategory Category { get; init; } = EDamageCategory.Status;

    /// <summary>
    /// Gets the power value of the move.
    /// </summary>
    /// <remarks>
    /// This property indicates the base power associated with a move in the system.
    /// The value is expected to be a non-negative integer, representing the move's strength during execution.
    /// A value of 0 typically represents moves that do not deal damage.
    /// </remarks>
    [PbsRange<int>(0)]
    public int Power { get; init; }

    /// <summary>
    /// Gets the accuracy of the move.
    /// </summary>
    /// <remarks>
    /// This property represents the likelihood, expressed as a percentage, that the move will successfully hit its target during execution.
    /// The value ranges from 0 to 100, where 100 signifies perfect accuracy, and lower values indicate a higher risk of missing.
    /// </remarks>
    [PbsRange<int>(0, 100)]
    public int Accuracy { get; init; } = 100;

    /// <summary>
    /// Gets the total Power Points (PP) available for the move.
    /// </summary>
    /// <remarks>
    /// This property indicates the maximum number of times a move can be used before it must be restored
    /// using items or other game mechanisms. The value is constrained to be at least 1.
    /// </remarks>
    [PbsRange<int>(1)]
    public int TotalPP { get; init; } = 5;

    /// <summary>
    /// Gets the priority level of the move.
    /// </summary>
    /// <remarks>
    /// This property determines the execution order of the move during battle.
    /// Moves with higher priority values are executed before those with lower values, regardless of the user's Speed stat.
    /// A priority level of 0 is considered standard, while positive or negative values modify that order.
    /// </remarks>
    public int Priority { get; init; }

    /// <summary>
    /// Gets the target type for the move.
    /// </summary>
    /// <remarks>
    /// This property specifies the intended target(s) of the move when used in battle.
    /// It defines if the move targets a single opponent, multiple opponents, the user, or other configurations.
    /// </remarks>
    public FName Target { get; init; }

    /// <summary>
    /// Gets the function code associated with the move.
    /// </summary>
    /// <remarks>
    /// This property represents a gameplay tag that defines the functional behavior or effect of a move
    /// in the Pokémon system. It is used to categorize and apply specific logic tied to the move's execution.
    /// </remarks>
    [PbsGameplayTag(IdentifierConstants.FunctionCodeTag, Create = true)]
    public FGameplayTag FunctionCode { get; init; }

    /// <summary>
    /// Gets the chance, expressed as a percentage, for the move's effect to successfully occur.
    /// </summary>
    /// <remarks>
    /// This property indicates the likelihood of a move's additional effect being applied during battle.
    /// Values range from 0 to 100 with 0 representing no chance and 100 ensuring the effect always occurs.
    /// </remarks>
    [PbsRange<int>(0)]
    public int EffectChance { get; init; }

    /// <summary>
    /// Gets the collection of gameplay tags associated with the move.
    /// </summary>
    /// <remarks>
    /// This property holds a container of gameplay tags that define various attributes, flags,
    /// or metadata relevant to a move. The tags can be utilized for categorization, filtering,
    /// or applying specific logic related to the move within the game system.
    /// </remarks>
    [PbsName("Flags")]
    [PbsGameplayTag("Data.Move", Create = true)]
    public FGameplayTagContainer Tags { get; init; } = new() { GameplayTags = [], ParentTags = [] };
}
