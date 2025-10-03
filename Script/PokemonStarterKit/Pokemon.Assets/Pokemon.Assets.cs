using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Assets;

/// <summary>
/// Represents the module responsible for managing the initialization and cleanup of Pokemon assets.
/// </summary>
/// <remarks>
/// This class implements the <see cref="IModuleInterface"/> interface to manage
/// the lifecycle of the module, including startup and shutdown processes.
/// </remarks>
public class FPokemonAssetsModule : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule() { }

    /// <inheritdoc />
    public void ShutdownModule() { }
}
