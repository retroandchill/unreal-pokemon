using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Battler;

/// <summary>
/// Represents a component for managing a battler's held item in a Pokemon battle.
/// This component allows querying and manipulating the held item of a battler entity.
/// </summary>
[UClass]
public class UBattlerHoldItemComponent : URPGComponent
{
    /// <summary>
    /// Represents the held item of a battler in a Pokemon battle.
    /// </summary>
    /// <remarks>
    /// This property provides access to the item currently held by the battler.
    /// The held item may influence the battler's actions, stats, effects, or interactions during a battle.
    /// </remarks>
    /// <value>
    /// An FItemHandle representing the held item, or null if no item is currently held.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|HoldItem")]
    public FItemHandle? HoldItem { get; set; }

    /// <summary>
    /// Indicates whether the battler currently holds an active item.
    /// </summary>
    /// <remarks>
    /// This property evaluates the held item of the battler and determines if it is active.
    /// An active item indicates that the battler's HoldItem is not null, which signifies
    /// that the battler has a held item that may influence gameplay in some capacity.
    /// </remarks>
    /// <value>
    /// True if the battler holds an active item; otherwise, false.
    /// </value>
    public bool ItemActive => HoldItem != null;
}
