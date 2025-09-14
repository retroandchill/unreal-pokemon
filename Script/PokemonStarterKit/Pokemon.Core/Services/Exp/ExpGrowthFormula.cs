using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

public interface IExpGrowthFormula
{
    static int MaxLevel => FGrowthRate.MaxLevel;

    FGrowthRateHandle GrowthRateFor { get; }

    int GetMinimumExpForLevel(int level);

    sealed int MaximumExp => GetMinimumExpForLevel(MaxLevel);

    sealed int AddExp(int exp1, int exp2) => Math.Clamp(exp1 + exp2, 0, MaximumExp);

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
