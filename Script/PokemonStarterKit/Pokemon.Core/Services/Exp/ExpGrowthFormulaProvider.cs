using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

[Service]
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
