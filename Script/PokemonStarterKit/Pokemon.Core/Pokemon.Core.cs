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
        FUnrealInjectModule.Instance.Services.RegisterServices();
    }

    public void ShutdownModule()
    {
        FUnrealInjectModule.Instance.Services.UnregisterServices();
    }
}
