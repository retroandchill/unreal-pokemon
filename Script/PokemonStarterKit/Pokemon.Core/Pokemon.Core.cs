using JetBrains.Annotations;
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
        var services = FUnrealInjectModule.Instance.Services;
        services.AddSingleton<IExpGrowthFormula, ErraticExpGrowthFormula>();
        services.AddSingleton<IExpGrowthFormula, FastExpGrowthFormula>();
        services.AddSingleton<IExpGrowthFormula, FluctuatingExpGrowthFormula>();
        services.AddSingleton<IExpGrowthFormula, MediumExpGrowthFormula>();
        services.AddSingleton<IExpGrowthFormula, ParabolicExpGrowthFormula>();
        services.AddSingleton<IExpGrowthFormula, SlowExpGrowthFormula>();
        services.AddSingleton<ExpGrowthFormulaProvider>();
        services.AddSingleton<IStatCalculationService, DefaultStatCalculationService>();
    }

    public void ShutdownModule()
    {
        FUnrealInjectModule
            .Instance.Services.RemoveAll<IExpGrowthFormula>()
            .RemoveAll<ExpGrowthFormulaProvider>()
            .RemoveAll<IStatCalculationService>();
    }
}
