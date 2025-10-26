using GameDataAccessTools.Core.Views;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Moves;

/// <summary>
/// Represents a Pokémon move, including its properties, effects, and metadata.
/// </summary>
[UClass]
public partial class UPokemonMove : UObject
{
    /// <summary>
    /// Represents the move associated with a Pokémon.
    /// This property encapsulates details such as the move's attributes, effects, and gameplay mechanics.
    /// </summary>
    /// <remarks>
    /// The move is defined as a handle to the underlying game data representing the move's behavior and metadata.
    /// Modifications to the move may impact related properties like total PP and associated gameplay characteristics.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Moves", BlueprintAccessors = true)]
    public FMoveHandle Move
    {
        get => Move_BackingField;
        set
        {
            Move_BackingField = value;
            PP = Math.Clamp(PP, 0, TotalPP);
        }
    }

    /// <summary>
    /// Represents the current Power Points (PP) of a Pokémon move.
    /// This property determines the remaining usages of the move in combat or other gameplay contexts.
    /// </summary>
    /// <remarks>
    /// The value is constrained between 0 and the move's total PP, ensuring it does not exceed the defined limits.
    /// Adjusting this property will automatically adhere to these constraints. The PP reflects the move's
    /// resourcefulness during battles and affects its availability.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Moves", BlueprintAccessors = true)]
    public int PP
    {
        get => PP_BackingField;
        set => PP_BackingField = Math.Clamp(value, 0, TotalPP);
    }

    /// <summary>
    /// Represents the count of PP Ups applied to a move.
    /// This property determines the enhancement of a move's total PP by increasing its maximum use limit.
    /// </summary>
    /// <remarks>
    /// The value of PPUps is clamped to a minimum of 0 and directly impacts the calculation of the move's TotalPP.
    /// Applying PP Ups to a move increases its longevity in battles by raising its PP ceiling.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "PP Ups", Category = "Moves", BlueprintAccessors = true)]
    public int PPUps
    {
        get => PPUps_BackingField;
        set => PPUps_BackingField = Math.Max(value, 0);
    }

    /// <summary>
    /// Represents the total Power Points (PP) available for a move.
    /// This value corresponds to the maximum PP of the move, including any applicable modifications such as PP Ups.
    /// </summary>
    /// <remarks>
    /// Total PP is calculated based on the base maximum PP of the move and any increase resulting from PP Ups.
    /// The value ensures that gameplay mechanics related to remaining usage of moves are properly enforced.
    /// </remarks>
    public int TotalPP
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get
        {
            var maxPP = Move.Entry.TotalPP;
            return maxPP + maxPP * PPUps / 5;
        }
    }

    /// <summary>
    /// Creates a new instance of the <c>UPokemonMove</c> class, initializes its properties with the provided
    /// parameters, and assigns the total PP value for the move.
    /// </summary>
    /// <param name="outer">The outer object used as the context for this move creation.</param>
    /// <param name="move">The handle of the move to be used for the creation of the <c>UPokemonMove</c>.</param>
    /// <returns>A newly created instance of the <c>UPokemonMove</c> initialized with the specified move.</returns>
    public static UPokemonMove Create(UObject outer, FMoveHandle move)
    {
        var newMove = NewObject<UPokemonMove>(outer);
        newMove.Move = move;
        newMove.PP = newMove.TotalPP;
        return newMove;
    }

    /// <summary>
    /// Represents the functionality code associated with a Pokémon move.
    /// This property defines the coded behavior or effect of the move during gameplay.
    /// </summary>
    /// <remarks>
    /// The functionality code serves as an identifier for the underlying logic that determines the move's specialized effects.
    /// It is directly linked to the move's game data and impacts the associated mechanics and outcomes in battle scenarios.
    /// </remarks>
    public FGameplayTag FunctionCode
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.FunctionCode;
    }

    /// <summary>
    /// Represents the power details of a Pokémon move.
    /// This property encapsulates the damage potential and type characteristics associated with the move.
    /// </summary>
    /// <remarks>
    /// The power is defined as structured data derived from the move's core attributes,
    /// influencing the overall effectiveness of the move during gameplay scenarios.
    /// Modifications to the underlying move may adjust its power-related dynamics.
    /// </remarks>
    public FDamageTypeData Power
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Damage;
    }

    /// <summary>
    /// Represents the elemental type of a Pokémon move.
    /// This property identifies the type associated with the move,
    /// which determines effectiveness and interactions with other types in battles.
    /// </summary>
    /// <remarks>
    /// The type is a core characteristic of a move, influencing critical gameplay mechanics such as damage multipliers,
    /// type matchups, and overall strategy. Changes to this property reflect the underlying data handled by the game.
    /// </remarks>
    public FTypeHandle Type
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Type;
    }

    /// <summary>
    /// Represents the damage category of a Pokémon move.
    /// This property determines whether a move is categorized as physical, special, or status-based.
    /// </summary>
    /// <remarks>
    /// The category impacts how a move interacts with a Pokémon's stats, such as Attack and Defense for physical moves,
    /// or Special Attack and Special Defense for special moves. Status moves typically apply non-damaging effects.
    /// </remarks>
    public EDamageCategory Category
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Category;
    }

    /// <summary>
    /// Indicates whether the move is categorized as a physical move.
    /// </summary>
    /// <remarks>
    /// A physical move typically deals damage based on the user's Attack stat
    /// and the opponent's Defense stat. This property reflects the move's
    /// classification within the game's mechanics.
    /// </remarks>
    public bool IsPhysical
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Physical", Category = "Moves")]
        get => Move.Entry.IsPhysical;
    }

    /// <summary>
    /// Indicates whether the Pokémon move is categorized as special.
    /// This property identifies moves that deal damage using the special attack stat of the user
    /// and the special defense stat of the target.
    /// </summary>
    /// <remarks>
    /// A special move typically corresponds to elemental or energy-based attacks,
    /// as opposed to physical moves, which involve direct physical contact,
    /// or status moves, which apply non-damaging effects.
    /// </remarks>
    public bool IsSpecial
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Physical", Category = "Moves")]
        get => Move.Entry.IsSpecial;
    }

    /// <summary>
    /// Indicates whether the move is categorized as a status move.
    /// This property determines if the move affects targets through non-damaging effects,
    /// such as altering stats, inflicting conditions, or providing utility advantages.
    /// </summary>
    /// <remarks>
    /// Status moves are distinct from physical or special moves and are defined by their
    /// lack of direct damage-dealing capabilities. Their effects vary widely and can
    /// influence battle mechanics including conditions, field states, or recovery actions.
    /// </remarks>
    public bool IsStatus
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Status", Category = "Moves")]
        get => Move.Entry.IsStatus;
    }

    /// <summary>
    /// Represents the accuracy of a Pokémon move.
    /// Indicates the likelihood of successfully executing the move during gameplay, expressed as a percentage.
    /// </summary>
    /// <remarks>
    /// The value is optional and may be undefined for moves that always hit their target.
    /// For such moves, the accuracy is not applicable and is represented as <see cref="Option{int}.None"/>.
    /// </remarks>
    public TOptional<int> Accuracy
    {
        get
        {
            var move = Move.Entry;
            return !move.AlwaysHits ? Accuracy : TOptional<int>.None;
        }
    }

    /// <summary>
    /// Represents the chance of a secondary effect being triggered by a Pokémon move.
    /// </summary>
    /// <remarks>
    /// EffectChance defines the probability, as a percentage, for non-guaranteed secondary effects of the move to occur.
    /// If the move has a guaranteed effect, this property will not apply and may return no value.
    /// </remarks>
    public TOptional<int> EffectChance
    {
        get
        {
            var move = Move.Entry;
            return !move.GuaranteedEffect ? EffectChance : TOptional<int>.None;
        }
    }

    /// <summary>
    /// Represents the target associated with a Pokémon move.
    /// This property defines the specific target(s) of a move during gameplay.
    /// </summary>
    /// <remarks>
    /// The target determines how a move interacts with single or multiple opponents and/or allies in a battle scenario.
    /// Modifications to the target may alter the move's effectiveness, scope, or mechanics during execution.
    /// </remarks>
    public FTargetHandle Target
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Target;
    }

    /// <summary>
    /// Represents the priority of the Pokémon move.
    /// Determines the execution order of the move during battle relative to other moves.
    /// </summary>
    /// <remarks>
    /// Priority is a numerical value that affects a move's turn order.
    /// Moves with higher priority execute before those with lower priority, regardless of speed.
    /// Zero is the standard priority value, while positive or negative values indicate higher or lower precedence respectively.
    /// </remarks>
    public int Priority
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Priority;
    }

    /// <summary>
    /// Represents the gameplay tags associated with a Pokémon move.
    /// This property provides metadata and classification information about move functionality and effects.
    /// </summary>
    /// <remarks>
    /// Tags are used to identify and categorize specific gameplay-related attributes of the move,
    /// enabling functionality like filtering, effect application, and compatibility checks.
    /// Modifications or retrieval of these tags can impact how the move is interpreted in various game mechanics.
    /// </remarks>
    public StructView<FGameplayTagContainer> Tags => Move.Entry.Tags;

    /// <summary>
    /// Represents the localized name of the Pokémon move.
    /// This property provides the in-game display name as a text value, which can vary based on localization settings.
    /// </summary>
    /// <remarks>
    /// The name of the move is sourced from the move's game data entry and reflects its official naming conventions and translations.
    /// Modifying this property may affect how the move is represented in user interfaces and gameplay contexts.
    /// </remarks>
    public FText Name
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Name;
    }

    /// <summary>
    /// Represents the textual description of the Pokémon move.
    /// This property provides a concise explanation of the move's functionality and effects in gameplay.
    /// </summary>
    /// <remarks>
    /// The description is sourced from the game's data and reflects the move's intended behavior.
    /// It may include details such as the move's mechanics, special effects, or situational applications.
    /// </remarks>
    public FText Description
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Description;
    }

    /// <summary>
    /// Indicates whether the move is classified as a hidden move.
    /// This property determines if a move is part of a special category often used outside of battle scenarios.
    /// </summary>
    /// <remarks>
    /// Hidden moves are typically linked to unique gameplay mechanics or world interaction features.
    /// Their availability and functionality may depend on specific conditions or progression within the game.
    /// </remarks>
    public bool IsHiddenMove
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Hidden Move", Category = "Moves")]
        get => Move.Entry.IsHiddenMove;
    }

    // TODO: These need to be updated to actually get the true display information

    /// <summary>
    /// Represents the display type associated with a Pokémon move.
    /// This property provides a handle to the type of the move as it is intended to be displayed in the game's user interface.
    /// </summary>
    /// <remarks>
    /// The display type is typically used for visual representation and may be influenced by certain game mechanics or specific overrides.
    /// It reflects the elemental or type association of the move as it appears to the player.
    /// </remarks>
    public FTypeHandle DisplayType
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Type;
    }

    /// <summary>
    /// Represents the damage category of a Pokémon move in a user-facing context.
    /// </summary>
    /// <remarks>
    /// This property reflects whether the move is classified as Physical, Special, or Status for the purpose of display.
    /// The value aligns with the move's internal category as defined in the game data.
    /// </remarks>
    public EDamageCategory DisplayCategory
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Category;
    }

    /// <summary>
    /// Represents the damage information of a Pokémon move for display purposes.
    /// This property retrieves standardized details related to the move's damage.
    /// </summary>
    /// <remarks>
    /// The display damage reflects the power value of the move, as defined in its core attributes.
    /// It serves as a visual or informational representation, separated from its internal game mechanics.
    /// </remarks>
    public FDamageTypeData DisplayDamage
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Power;
    }

    /// <summary>
    /// Provides the accuracy of the Pokémon move as it is displayed in the game.
    /// </summary>
    /// <remarks>
    /// The display accuracy reflects the chance of a move to successfully hit the target under normal conditions.
    /// This value may differ from internal calculations if any modifiers or conditions are applied.
    /// </remarks>
    public TOptional<int> DisplayAccuracy
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Accuracy;
    }
}
