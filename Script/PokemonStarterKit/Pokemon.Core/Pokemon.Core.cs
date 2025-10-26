using JetBrains.Annotations;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Pokemon.Core.Services.Exp;
using Pokemon.Core.Services.Stats;
using UnrealInject;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace Pokemon.Core;

/// <summary>
/// LogPokemonCore serves as a centralized logging utility specific to the Pokemon Core module.
/// This class is used for logging purposes by various component savers and other classes
/// within the module, allowing consistent and organized log management.
/// It is decorated with the <see cref="CustomLogAttribute"/>, enabling UnrealSharp logging capabilities.
/// </summary>
[CustomLog]
public static partial class LogPokemonCore;

/// <summary>
/// FPokemonCoreModule is responsible for managing the lifecycle of the Pokemon Core module in an Unreal Engine project.
/// It implements the <see cref="IModuleInterface"/> interface, providing module setup and teardown behaviors.
/// </summary>
public class FPokemonCoreModule : IModuleInterface
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
