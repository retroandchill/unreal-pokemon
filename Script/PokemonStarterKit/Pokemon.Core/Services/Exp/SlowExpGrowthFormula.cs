using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Represents the specific experience growth formula for Pokémon
/// that adhere to the "Slow" experience growth rate.
/// The formula calculates the minimum experience points required
/// to reach a certain level based on a cubic growth function scaled by a factor.
/// </summary>
[Service]
public class SlowExpGrowthFormula : IExpGrowthFormula
{
    /// <inheritdoc />
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Slow;

    /// <inheritdoc />
    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level == 1 ? 0 : (int)Math.Pow(level, 3) * 5 / 4;
    }
}
