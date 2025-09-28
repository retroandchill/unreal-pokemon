using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Core.Services.Async;
using Pokemon.UI.Services.Async;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.UI;

public class FPokemonUIModule : IModuleInterface
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
