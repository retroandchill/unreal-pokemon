using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

public class ParabolicExpGrowthFormula : IExpGrowthFormula
{
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Parabolic;

    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level == 1
            ? 0
            : (int)Math.Pow(level, 3) * 6 / 5 - 15 * (int)Math.Pow(level, 2) + 100 * level - 140;
    }
}
