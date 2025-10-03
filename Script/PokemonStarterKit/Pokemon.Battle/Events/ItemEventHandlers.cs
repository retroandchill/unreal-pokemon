using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Events;

/// <summary>
/// Represents a service that manages and executes event handlers related to item-based speed calculations
/// in battles.
/// </summary>
[Service]
public class ItemEventHandlers
{
    /// <summary>
    /// Delegate representing a function that calculates the modified speed value for a battler in battle.
    /// </summary>
    public Dictionary<FItemHandle, SpeedCalc> SpeedCalc { get; } = new();

    /// <summary>
    /// Applies an item-based speed calculation for a given battler, based on its held item's effects
    /// and a provided multiplier, if applicable.
    /// </summary>
    /// <param name="ability">The handle representing the item's unique ability, which determines the speed calculation logic.</param>
    /// <param name="battler">The battler entity whose speed is being calculated.</param>
    /// <param name="multiplier">The initial speed multiplier to be modified based on the item's effects.</param>
    /// <returns>The modified speed multiplier after applying the item's effects, or the original multiplier if no calculation is defined.</returns>
    public float TriggerSpeedCalc(FItemHandle ability, UBattler battler, float multiplier)
    {
        return SpeedCalc.TryGetValue(ability, out var calc) ? calc(battler, multiplier) : multiplier;
    }
}
