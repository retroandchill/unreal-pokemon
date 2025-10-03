using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Interface representing the formula for experience growth in Pokémon systems.
/// </summary>
public interface IExpGrowthFormula
{
    /// <summary>
    /// Represents the maximum level a Pokémon can achieve, typically defined
    /// in the game configuration.
    /// </summary>
    static int MaxLevel => FGrowthRate.MaxLevel;

    /// <summary>
    /// Represents the specific growth rate identifier associated with an experience growth formula,
    /// which determines how experience points (EXP) requirements scale for leveling up.
    /// </summary>
    FGrowthRateHandle GrowthRateFor { get; }

    /// <summary>
    /// Calculates the minimum amount of experience required to reach a specific level
    /// based on the growth rate formula of the Pokémon.
    /// </summary>
    /// <param name="level">The desired level for which the minimum experience is calculated. Must be a non-negative integer.</param>
    /// <returns>The minimum experience needed to reach the specified level.</returns>
    /// <exception cref="ArgumentOutOfRangeException">Thrown if the level is less than 0.</exception>
    int GetMinimumExpForLevel(int level);

    /// <summary>
    /// Represents the maximum amount of experience a Pokémon can accumulate,
    /// corresponding to the experience required to reach the highest level defined by the growth rate formula.
    /// </summary>
    sealed int MaximumExp => GetMinimumExpForLevel(MaxLevel);

    /// <summary>
    /// Adds two experience point values and ensures the result does not exceed the maximum allowed experience
    /// or drop below zero.
    /// </summary>
    /// <param name="exp1">The first experience value to be added. Must be a non-negative integer.</param>
    /// <param name="exp2">The second experience value to be added. Must be a non-negative integer.</param>
    /// <returns>The clamped sum of the two experience values, ensuring it is between 0 and the maximum experience limit.</returns>
    sealed int AddExp(int exp1, int exp2) => Math.Clamp(exp1 + exp2, 0, MaximumExp);

    /// <summary>
    /// Determines the level a Pokémon has reached given a specific amount of experience
    /// based on the growth rate formula of the Pokémon.
    /// </summary>
    /// <param name="exp">The amount of experience points. Must be a non-negative integer.</param>
    /// <returns>The level corresponding to the given experience points.</returns>
    /// <exception cref="ArgumentOutOfRangeException">Thrown if the experience points are less than 0.</exception>
    sealed int GetLevelForExp(int exp)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(exp, 0);
        var max = MaxLevel;
        if (exp >= MaximumExp)
            return max;

        for (var i = 0; i <= max; i++)
        {
            if (exp < GetMinimumExpForLevel(i))
                return i - 1;
        }

        return max;
    }
}
