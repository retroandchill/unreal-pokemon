using Pokemon.Battle.Entities;
using Pokemon.Core.Entities;
using RPG.Battle.Actions;
using UnrealSharp.Attributes;

namespace Pokemon.Battle.Actions;

/// <summary>
/// Represents a target for an action within the battle system.
/// </summary>
/// <param name="SideIndex">The index of the side that the target belongs to.</param>
/// <param name="TargetIndex">The index of the target within the side.</param>
[UStruct]
public readonly partial record struct FActionTarget(
    [UProperty(PropertyFlags.BlueprintReadOnly)] int SideIndex,
    [UProperty(PropertyFlags.BlueprintReadOnly)] int TargetIndex
)
{
    /// <summary>
    /// Represents a default, uninitialized, or no-op state for this data type.
    /// </summary>
    public static FActionTarget None => new(-1, -1);
}

/// <summary>
/// Represents an action that can be performed in a Pokémon battle.
/// </summary>
/// <remarks>
/// This interface defines the primary structure for Pokémon-specific actions within the battle system,
/// including their execution order (via priority) and the entity performing the action.
/// </remarks>
public interface IPokemonAction : IAction
{
    /// <summary>
    /// Defines the priority level of an action within the Pokémon battle system.
    /// </summary>
    /// <remarks>
    /// Higher priority values indicate that the action should be executed earlier in the turn order.
    /// Actions with the same priority level will use other factors, such as the user's effective speed,
    /// to determine execution order.
    /// </remarks>
    int Priority { get; }

    /// <summary>
    /// Gets the battler that is performing the Pokémon action.
    /// </summary>
    /// <remarks>
    /// This represents the entity (battler) responsible for executing the action in the context of a Pokémon battle.
    /// It is used to determine the order of actions, resolve effects, and calculate battle outcomes.
    /// </remarks>
    UBattler User { get; }
}
