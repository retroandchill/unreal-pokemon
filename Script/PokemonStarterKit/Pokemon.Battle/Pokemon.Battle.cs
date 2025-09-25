using Microsoft.Extensions.DependencyInjection;
using Pokemon.Battle.Services;
using RPG.Battle.Services;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Battle;

public class FPokemonBattleModule : IModuleInterface
{
    public void StartupModule()
    {
        FUnrealInjectModule.Instance.Services.AddScoped<
            ITurnOrderService,
            PokemonTurnOrderService
        >();
    }

    public void ShutdownModule() { }
}
