using Pokemon.Battle.Entities;
using Pokemon.SourceGenerators.Attributes;

namespace Pokemon.Battle.Events;

public static class AbilityEventRegistrations
{
    [BattleAbilityEvent(nameof(AbilityEvents.SpeedCalc), "QUICKFEET")]
    public static float QuickFeet(UBattler battler, float multiplier)
    {
        return battler.StatusEffect.HasValue ? multiplier * 1.5f : multiplier;
    }
}
