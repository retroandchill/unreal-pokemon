using Pokemon.Battle.Entities;
using UnrealSharp.Attributes;
using UnrealSharp.Engine;

namespace Pokemon.Battle.Actors;

/// <summary>
/// Represents the context of a battle in the Pokemon battle system.
/// </summary>
/// <remarks>
/// This abstract class is used as a base for managing and providing
/// contextual information about the ongoing battle, including access
/// to all entities involved in the battle.
/// </remarks>
[UClass]
public class ABattleContext : AInfo
{
    /// <summary>
    /// Provides an enumerable collection of all battlers participating in the battle.
    /// </summary>
    /// <remarks>
    /// This property allows access to each entity currently involved in the battle, such as Pokémon or other combatants.
    /// Typically used to evaluate and manipulate battlers during battle operations, including turn order
    /// determination, effects application, and interactions.
    /// </remarks>
    /// <returns>
    /// An <see cref="IEnumerable{UBattler}"/> representing all entities actively part of the battle.
    /// </returns>
    public IEnumerable<UBattler> AllBattlers => throw new NotImplementedException();
}
