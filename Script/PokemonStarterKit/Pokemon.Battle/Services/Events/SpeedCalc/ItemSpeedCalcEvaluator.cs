using Pokemon.Battle.Entities;
using Pokemon.Battle.Events;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

/// <summary>
/// Evaluates the impact of item-related abilities on the speed multiplier of a battler during a speed calculation.
/// </summary>
/// <remarks>
/// This class is used as part of the speed calculation system in battles. It leverages the provided ability handler
/// to determine how an ability, when active, affects the speed multiplier of a given battler. The evaluation applies
/// only if the battler's ability is active and assigned.
/// </remarks>
[Service]
public class ItemSpeedCalcEvaluator([ReadOnly] AbilityEventHandlers handlers) : ISpeedCalcEvaluator
{
    /// <inheritdoc />
    public int Priority => SpeedCalcPriorities.Ability;

    /// <inheritdoc />
    public void Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler is { AbilityActive: true, Ability: not null })
        {
            multiplier = handlers.TriggerSpeedCalc(battler.Ability.Value, battler, multiplier);
        }
    }
}
