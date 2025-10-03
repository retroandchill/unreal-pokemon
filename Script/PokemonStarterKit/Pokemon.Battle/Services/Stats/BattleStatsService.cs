using System.Numerics;
using Pokemon.Battle.Entities;
using Pokemon.Data.Model.HardCoded;

namespace Pokemon.Battle.Services.Stats;

/// <summary>
/// Interface defining the methods and properties for calculating and managing
/// battle statistics within the Pokemon battle system.
/// </summary>
public interface IBattleStatsService
{
    /// <summary>
    /// Gets the maximum allowable stage for any stat adjustment in the battle system.
    /// The stat stage impacts the effectiveness of a battler's stat, and this property
    /// defines the upper limit of stages a stat can reach.
    /// </summary>
    int StatStageMaximum { get; }

    /// <summary>
    /// Calculates the change in a stat value based on the specified stat stage.
    /// </summary>
    /// <typeparam name="T">The numeric type of the values being calculated, implementing <see cref="System.Numerics.INumber{T}"/>.</typeparam>
    /// <param name="stat">The handle representing the stat to modify.</param>
    /// <param name="baseValue">The base value of the stat before applying the stage change.</param>
    /// <param name="stage">The stage value representing the adjustment to the stat (positive or negative).</param>
    /// <returns>The adjusted stat value after applying the stage change.</returns>
    T CalculateStatStageChange<T>(FBattleStatHandle stat, T baseValue, int stage)
        where T : INumber<T>;

    /// <summary>
    /// Calculates the effective speed of a battler, taking into account all modifiers
    /// such as stat changes, abilities, items, and status effects.
    /// </summary>
    /// <param name="battler">The battler whose effective speed is being calculated.</param>
    /// <returns>The effective speed of the specified battler after applying all relevant modifiers.</returns>
    int GetEffectiveSpeed(UBattler battler);
}
