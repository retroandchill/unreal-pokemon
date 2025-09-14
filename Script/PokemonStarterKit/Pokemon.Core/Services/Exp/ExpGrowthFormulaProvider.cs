using Pokemon.Data.Model.HardCoded;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

public class ExpGrowthFormulaProvider
{
    private readonly Dictionary<FGrowthRateHandle, IExpGrowthFormula> _expGrowthFormulas;

    public ExpGrowthFormulaProvider(IEnumerable<IExpGrowthFormula> expGrowthFormulas)
    {
        _expGrowthFormulas = expGrowthFormulas.ToDictionary(x => x.GrowthRateFor);
    }

    public IExpGrowthFormula GetGrowthFormula(FGrowthRateHandle growthRate)
    {
        return _expGrowthFormulas[growthRate];
    }
}
