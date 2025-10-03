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

/// <summary>
/// The FPokemonBattleModule class is an implementation of the IModuleInterface,
/// designed to manage and inject services related to Pokémon battle functionality.
/// It relies on the UnrealInject framework to handle service registration and activation during the lifecycle of the module.
/// </summary>
public class FPokemonBattleModule : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule()
    {
        FUnrealInjectModule.Instance.Services.RegisterServices().RegisterAbilityEvents();
    }

    /// <inheritdoc />
    public void ShutdownModule()
    {
        FUnrealInjectModule.Instance.Services.UnregisterServices().UnregisterAbilityEvents();
    }
}
