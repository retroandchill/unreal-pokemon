using System.Collections.Immutable;
using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Abilities;

[Service]
public class DefaultBattleAbilityService(
    IEnumerable<IAbilitySuppressionEvaluator> suppressionEvaluators
) : IBattleAbilityService
{
    private readonly ImmutableArray<IAbilitySuppressionEvaluator> _suppressionEvaluators =
    [
        .. suppressionEvaluators,
    ];

    public bool HasActiveAbility(
        UBattler battler,
        bool ignoreFained = false,
        FAbilityHandle checkAbility = default
    )
    {
        if (battler.IsFainted && !ignoreFained)
            return false;

        return !_suppressionEvaluators.Any(s => s.IsAbilitySuppressed(battler, checkAbility));
    }
}
