using Microsoft.Extensions.Options;
using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Events;

[Service]
public class AbilityEvents
{
    public Dictionary<FAbilityHandle, SpeedCalc> SpeedCalc { get; } = new();
}

[Service]
public class AbilityEventHandlers(IOptions<AbilityEvents> events)
{
    private readonly AbilityEvents _abilityEvents = events.Value;

    public float TriggerSpeedCalc(FAbilityHandle ability, UBattler battler, float multiplier = 1.0f)
    {
        return _abilityEvents.SpeedCalc.TryGetValue(ability, out var calc)
            ? calc(battler, multiplier)
            : multiplier;
    }
}
