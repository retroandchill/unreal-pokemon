using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Battle.Services;
using Pokemon.Battle.Services.Abilities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using Pokemon.Battle.Services.Stats;
using RPG.Battle.Services;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Battle;

public class FPokemonBattleModule : IModuleInterface
{
    public void StartupModule()
    {
        var services = FUnrealInjectModule.Instance.Services;
        services.AddSingleton<ITurnOrderService, PokemonTurnOrderService>();
        services.AddSingleton<IBattleStatsService, DefaultBattleStatsService>();
        services.AddSingleton<IBattleAbilityService, DefaultBattleAbilityService>();
        services.AddSingleton<ISpeedCalcEvaluator, AbilitySpeedCalcEvaluator>();
    }

    public void ShutdownModule()
    {
        FUnrealInjectModule
            .Instance.Services.RemoveAll<ITurnOrderService>()
            .RemoveAll<IBattleStatsService>()
            .RemoveAll<IBattleAbilityService>()
            .RemoveAll<ISpeedCalcEvaluator>();
    }
}
