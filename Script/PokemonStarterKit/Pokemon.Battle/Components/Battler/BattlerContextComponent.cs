using Pokemon.Battle.Actors;
using Pokemon.Battle.Entities;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Battler;

/// <summary>
/// Represents a component responsible for managing the battler's context within a battle system.
/// </summary>
[UClass]
public partial class UBattlerContextComponent : URPGComponent
{
    /// <summary>
    /// Gets the owning active side associated with this battler context.
    /// </summary>
    /// <remarks>
    /// This property represents the <see cref="UActiveSide"/> instance that serves as the owner of the current
    /// battler context component. The owning side is used to determine the context and behavior of the battler
    /// during the battle, including accessing the <see cref="ABattleContext"/>.
    /// </remarks>
    /// <value>
    /// A readonly instance of <see cref="UActiveSide"/> representing the active side that owns this component.
    /// </value>
    [UProperty(PropertyFlags.Transient)]
    public partial UActiveSide OwningSide { get; private set; }

    /// <summary>
    /// Gets the battle context associated with the owning active side of this battler context component.
    /// </summary>
    /// <remarks>
    /// This property retrieves the <see cref="ABattleContext"/> instance from the <see cref="UActiveSide"/>
    /// that owns the current battler context component. The battle context provides detailed information
    /// and control over the ongoing battle scenario.
    /// </remarks>
    /// <value>
    /// An instance of <see cref="ABattleContext"/> representing the battle context tied to the owning side of this component.
    /// </value>
    public ABattleContext BattleContext => OwningSide.BattleContext;
}
