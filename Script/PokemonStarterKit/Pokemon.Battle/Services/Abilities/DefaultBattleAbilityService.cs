using System.Collections.Immutable;
using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Abilities;

/// <summary>
/// Represents the default implementation of the battle ability service.
/// Provides functionality to determine whether a battler has an active ability,
/// considering any suppression evaluators that may affect it.
/// </summary>
/// <param name="suppressionEvaluators">The collection of suppression evaluators to use.</param>
[Service]
public class DefaultBattleAbilityService(IEnumerable<IAbilitySuppressionEvaluator> suppressionEvaluators)
    : IBattleAbilityService
{
    private readonly ImmutableArray<IAbilitySuppressionEvaluator> _suppressionEvaluators = [.. suppressionEvaluators];

    /// <inheritdoc />
    public bool HasActiveAbility(UBattler battler, bool ignoreFained = false, FAbilityHandle checkAbility = default)
    {
        if (battler.IsFainted && !ignoreFained)
            return false;

        return !_suppressionEvaluators.Any(s => s.IsAbilitySuppressed(battler, checkAbility));
    }
}
