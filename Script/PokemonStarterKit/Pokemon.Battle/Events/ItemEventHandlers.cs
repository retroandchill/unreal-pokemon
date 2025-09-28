using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Events;

[Service]
public class ItemEventHandlers
{
    public Dictionary<FItemHandle, SpeedCalc> SpeedCalc { get; } = new();

    public float TriggerSpeedCalc(FItemHandle ability, UBattler battler, float multiplier)
    {
        return SpeedCalc.TryGetValue(ability, out var calc)
            ? calc(battler, multiplier)
            : multiplier;
    }
}