using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Provides a centralized mechanism for retrieving experience growth formulas
/// based on a specified growth rate.
/// </summary>
[Service]
public class ExpGrowthFormulaProvider
{
    private readonly Dictionary<FGrowthRateHandle, IExpGrowthFormula> _expGrowthFormulas;

    /// <summary>
    /// Provides a centralized mechanism for retrieving experience growth formulas
    /// based on a specified growth rate.
    /// </summary>
    /// <param name="expGrowthFormulas">The collection of experience growth formulas to use.</param>
    public ExpGrowthFormulaProvider(IEnumerable<IExpGrowthFormula> expGrowthFormulas)
    {
        _expGrowthFormulas = expGrowthFormulas.ToDictionary(x => x.GrowthRateFor);
    }

    /// <summary>
    /// Retrieves the experience growth formula associated with a specific growth rate.
    /// </summary>
    /// <param name="growthRate">The growth rate handle used to identify the corresponding experience growth formula.</param>
    /// <returns>The experience growth formula corresponding to the provided growth rate.</returns>
    public IExpGrowthFormula GetGrowthFormula(FGrowthRateHandle growthRate)
    {
        return _expGrowthFormulas[growthRate];
    }
}
