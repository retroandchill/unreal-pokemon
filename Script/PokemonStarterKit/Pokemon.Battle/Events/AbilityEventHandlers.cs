using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;

namespace Pokemon.Battle.Events;

public class AbilityEventHandlers
{
    public Dictionary<FAbilityHandle, SpeedCalc> SpeedCalc { get; } = new();

    public float TriggerSpeedCalc(FAbilityHandle ability, UBattler battler, float multiplier)
    {
        return SpeedCalc.TryGetValue(ability, out var calc)
            ? calc(battler, multiplier)
            : multiplier;
    }
}
