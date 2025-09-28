using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Events;
using Pokemon.Battle.Services;
using Pokemon.Battle.Services.Abilities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using Pokemon.Battle.Services.Stats;
using Pokemon.Data.Model.PBS;
using RPG.Battle.Services;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Battle;

public class FPokemonBattleModule : IModuleInterface
{
    public void StartupModule()
    {
        FUnrealInjectModule.Instance.Services.RegisterServices().RegisterAbilityEvents();
    }

    public void ShutdownModule()
    {
        FUnrealInjectModule.Instance.Services.UnregisterServices().UnregisterAbilityEvents();
    }
}
