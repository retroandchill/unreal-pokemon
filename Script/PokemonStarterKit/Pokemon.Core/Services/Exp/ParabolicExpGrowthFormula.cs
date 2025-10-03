using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Represents an implementation of the parabolic experience growth formula for calculating the
/// experience points required for leveling up in a system adhering to the parabolic growth rate.
/// </summary>
/// <remarks>
/// The parabolic growth formula is a specific mathematical growth pattern used to calculate experience
/// points (EXP) required to reach a specific level. It is implemented as an approximation based on the
/// cubic scaling of the level, with modifications to account for the parabolic characteristics.
/// This class enforces a non-negative level input and is optimized for systems conforming to the
/// <see cref="FGrowthRate.Parabolic" /> growth rate.
/// </remarks>
[Service]
public class ParabolicExpGrowthFormula : IExpGrowthFormula
{
    /// <inheritdoc />
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Parabolic;

    /// <inheritdoc />
    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level == 1 ? 0 : (int)Math.Pow(level, 3) * 6 / 5 - 15 * (int)Math.Pow(level, 2) + 100 * level - 140;
    }
}
