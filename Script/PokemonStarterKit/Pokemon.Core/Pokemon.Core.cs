using JetBrains.Annotations;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Core.Services.Exp;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Core;

[UsedImplicitly]
public class FPokemonCoreModule : IModuleInterface
{
    public void StartupModule()
    {
        FUnrealInjectModule.Instance.ConfigureServices(services =>
        {
            services
                .AddSingleton<IExpGrowthFormula, ErraticExpGrowthFormula>()
                .AddSingleton<IExpGrowthFormula, FastExpGrowthFormula>()
                .AddSingleton<IExpGrowthFormula, FluctuatingExpGrowthFormula>()
                .AddSingleton<IExpGrowthFormula, MediumExpGrowthFormula>()
                .AddSingleton<IExpGrowthFormula, ParabolicExpGrowthFormula>()
                .AddSingleton<IExpGrowthFormula, SlowExpGrowthFormula>()
                .AddSingleton<ExpGrowthFormulaProvider>();
        });
    }

    public void ShutdownModule()
    {
        FUnrealInjectModule.Instance.ConfigureServices(services =>
        {
            services.RemoveAll<IExpGrowthFormula>().RemoveAll<ExpGrowthFormulaProvider>();
        });
    }
}
