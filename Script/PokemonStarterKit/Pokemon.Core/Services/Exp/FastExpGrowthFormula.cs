using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Represents the fast experience growth formula used for calculating experience points (EXP)
/// required for a Pokémon to reach a specific level.
/// </summary>
/// <remarks>
/// This formula is used in the Pokémon game mechanics for Pokémon whose experience
/// growth rate is categorized as "Fast".
/// </remarks>
[Service]
public class FastExpGrowthFormula : IExpGrowthFormula
{
    /// <inheritdoc />
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Fast;

    /// <inheritdoc />
    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level == 1 ? 0 : (int)Math.Pow(level, 3) * 4 / 5;
    }
}
