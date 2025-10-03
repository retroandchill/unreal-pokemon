using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;

namespace Pokemon.Battle.Services.Abilities;

/// <summary>
/// Provides an interface to evaluate whether a specific ability of a battler is suppressed.
/// This is useful in determining ability effects within a Pokemon battle context.
/// </summary>
public interface IAbilitySuppressionEvaluator
{
    /// <summary>
    /// Determines whether a specific ability of a battler is suppressed.
    /// </summary>
    /// <param name="battler">The battler whose ability suppression status is being evaluated.</param>
    /// <param name="checkAbility">The ability to check for suppression. If not specified, checks for suppression in general.</param>
    /// <returns>True if the ability is suppressed; otherwise, false.</returns>
    bool IsAbilitySuppressed(UBattler battler, FAbilityHandle checkAbility = default);
}
