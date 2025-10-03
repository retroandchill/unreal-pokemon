using Pokemon.Battle.Actors;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Side;

/// <summary>
/// Represents the active side context component associated with a battle side in the Pokemon battle system.
/// </summary>
/// <remarks>
/// This component manages the contextual relationship between a battle side and the overarching battle context.
/// It is a transient component, meaning it exists only for the duration of the battle lifecycle.
/// </remarks>
public class UActiveSideContextComponent : URPGComponent
{
    /// <summary>
    /// Represents the context of an active Pokemon battle within the gameplay framework.
    /// </summary>
    /// <remarks>
    /// This property refers to the overarching battle context in which the current battle side
    /// interacts. It provides access to global battle details and allows for communication
    /// between all participating entities and systems within the battle.
    /// The property is transient, meaning it only persists for the duration of the battle.
    /// </remarks>
    [UProperty(PropertyFlags.Transient)]
    public ABattleContext BattleContext { get; private set; }
}
