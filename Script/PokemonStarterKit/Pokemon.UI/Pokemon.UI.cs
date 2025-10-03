using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Core.Services.Async;
using Pokemon.UI.Services.Async;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.UI;

/// <summary>
/// Represents the UI module for the Pokemon application.
/// </summary>
/// <remarks>
/// This module acts as an entry point for initializing and shutting down
/// the services related to the Pokemon UI. It interacts with the
/// <see cref="UnrealInject.FUnrealInjectModule"/> to register and unregister
/// relevant services during the application lifecycle.
/// </remarks>
public class FPokemonUIModule : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule()
    {
        FUnrealInjectModule.Instance.Services.RegisterServices();
    }

    /// <inheritdoc />
    public void ShutdownModule()
    {
        FUnrealInjectModule.Instance.Services.UnregisterServices();
    }
}
