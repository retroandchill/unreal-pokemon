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
        FUnrealInjectModule
            .Instance.Services.AddSingleton<ITurnOrderService, PokemonTurnOrderService>()
            .AddSingleton<IBattleStatsService, DefaultBattleStatsService>()
            .AddSingleton<IBattleAbilityService, DefaultBattleAbilityService>()
            .AddSingleton<ISpeedCalcEvaluator, AbilitySpeedCalcEvaluator>();
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
