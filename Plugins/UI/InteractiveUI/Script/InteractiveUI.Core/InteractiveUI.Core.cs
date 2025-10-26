using JetBrains.Annotations;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace InteractiveUI.Core;

/// <summary>
/// Provides custom logging capabilities for Interactive UI Core components.
/// </summary>
/// <remarks>
/// This static class serves as a centralized logging mechanism for diagnostics, warnings, and error messages
/// specifically related to the Interactive UI Core module.
/// Intended to streamline log output consistency and maintainability across the module's implementation.
/// </remarks>
[CustomLog]
public static partial class LogInteractiveUICore;

/// <summary>
/// Represents the core module for the Interactive UI system.
/// </summary>
/// <remarks>
/// This class implements the <see cref="IModuleInterface"/> and is responsible for handling the
/// initialization and shutdown operations for the Interactive UI Core module.
/// It serves as the base module class to ensure proper lifecycle management within the system.
/// </remarks>
public class FInteractiveUICoreModule : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule() { }

    /// <inheritdoc />
    public void ShutdownModule() { }
}
