using JetBrains.Annotations;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Core.Services.Exp;
using Pokemon.Core.Services.Stats;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace Pokemon.Core;

[CustomLog]
public static partial class LogPokemonCore;

[UsedImplicitly]
public class FPokemonCoreModule : IModuleInterface
{
    public void StartupModule()
    {
        FUnrealInjectModule
            .Instance.Services.AddSingleton<IExpGrowthFormula, ErraticExpGrowthFormula>()
            .AddSingleton<IExpGrowthFormula, FastExpGrowthFormula>()
            .AddSingleton<IExpGrowthFormula, FluctuatingExpGrowthFormula>()
            .AddSingleton<IExpGrowthFormula, MediumExpGrowthFormula>()
            .AddSingleton<IExpGrowthFormula, ParabolicExpGrowthFormula>()
            .AddSingleton<IExpGrowthFormula, SlowExpGrowthFormula>()
            .AddSingleton<ExpGrowthFormulaProvider>()
            .AddSingleton<IStatCalculationService, DefaultStatCalculationService>();
    }

    public void ShutdownModule()
    {
        FUnrealInjectModule
            .Instance.Services.RemoveAll<IExpGrowthFormula>()
            .RemoveAll<ExpGrowthFormulaProvider>()
            .RemoveAll<IStatCalculationService>();
    }
}
