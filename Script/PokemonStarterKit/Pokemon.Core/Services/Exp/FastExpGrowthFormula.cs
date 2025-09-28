using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

[Service]
public class FastExpGrowthFormula : IExpGrowthFormula
{
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Fast;

    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level == 1 ? 0 : (int)Math.Pow(level, 3) * 4 / 5;
    }
}
