using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Exp;

/// <summary>
/// Represents the experience growth formula for "Erratic" growth rate in the context of the Pokémon core services.
/// Implements the <see cref="IExpGrowthFormula"/> interface.
/// </summary>
/// <remarks>
/// This formula determines the required experience points for a Pokémon to reach a specific level,
/// according to the "Erratic" growth rate pattern. Calculations vary depending on the Pokémon's level
/// and follow specified mathematical expressions for different level ranges.
/// </remarks>
[Service]
public class ErraticExpGrowthFormula : IExpGrowthFormula
{
    /// <inheritdoc />
    public FGrowthRateHandle GrowthRateFor => FGrowthRate.Erratic;

    /// <inheritdoc />
    public int GetMinimumExpForLevel(int level)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(level, 0);
        return level switch
        {
            1 => 0,
            <= 50 => (int)Math.Pow(level, 3) * (100 - level) / 50,
            <= 68 => (int)Math.Pow(level, 3) * (150 - level) / 100,
            <= 98 => (int)Math.Pow(level, 3) * ((1911 - 10 * level) / 3) / 500,
            _ => (int)Math.Pow(level, 3) * (160 - level) / 100,
        };
    }
}
