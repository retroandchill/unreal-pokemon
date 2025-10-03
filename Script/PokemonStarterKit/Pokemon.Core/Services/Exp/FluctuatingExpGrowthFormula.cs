using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Represents the Fluctuating experience growth formula used to calculate the minimum
/// experience points required for a Pokémon to reach a specific level.
/// Adheres to the "Fluctuating" growth rate category.
/// </summary>
[Service]
public class FluctuatingExpGrowthFormula : IExpGrowthFormula
{
    /// <inheritdoc />
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Fluctuating;

    /// <inheritdoc />
    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level switch
        {
            1 => 0,
            <= 15 => (int)Math.Pow(level, 3) * (24 + (level + 1) / 3) / 50,
            <= 35 => (int)Math.Pow(level, 3) * (14 + level) / 50,
            _ => (int)Math.Pow(level, 3) * (32 + level / 2) / 50,
        };
    }
}
