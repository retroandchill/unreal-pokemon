using Pokemon.Battle.Entities;
using Pokemon.SourceGenerators.Attributes;

namespace Pokemon.Battle.Events;

/// <summary>
/// Provides static methods for handling ability-based event registrations in Pokémon battles.
/// </summary>
/// <remarks>
/// This class primarily registers and defines ability-related event handlers used during specific gameplay situations in Pokémon battles.
/// </remarks>
public static class AbilityEventRegistrations
{
    /// <summary>
    /// Modifies the speed multiplier of a Pokémon with the "Quick Feet" ability based on its status effect.
    /// </summary>
    /// <param name="battler">The battler whose speed is being calculated. Represents a Pokémon in the battle.</param>
    /// <param name="multiplier">The current speed multiplier to be modified.</param>
    /// <returns>Returns the updated speed multiplier. If the battler has a status effect, the multiplier is increased by 50%; otherwise, the initial multiplier is returned unchanged.</returns>
    [BattleAbilityEvent(nameof(AbilityEvents.SpeedCalc), "QUICKFEET")]
    public static float QuickFeet(UBattler battler, float multiplier)
    {
        return battler.StatusEffect.HasValue ? multiplier * 1.5f : multiplier;
    }
}
