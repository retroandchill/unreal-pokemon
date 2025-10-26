using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace GameDataAccessTools.Core;

/// <summary>
/// Provides logging functionalities for the core operations of game data access tools.
/// Utilized to streamline logging practices and improve debugging across the game data access functionalities.
/// </summary>
[CustomLog]
public static partial class LogGameDataAccessToolsCore;

/// <summary>
/// Provides functionalities to manage game data access within the framework.
/// Implements <see cref="IModuleInterface"/> to handle module-specific startup and shutdown operations.
/// </summary>
public class FManagedGameDataAccessTools : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule() { }

    /// <inheritdoc />
    public void ShutdownModule() { }
}
