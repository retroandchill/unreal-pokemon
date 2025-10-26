using JetBrains.Annotations;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Editor;

/// <summary>
/// Represents the Pokemon editor module. This module serves as an interface
/// for initialization and cleanup tasks specific to the Pokemon editor environment.
/// </summary>
public class FPokemonEditorModule : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule() { }

    /// <inheritdoc />
    public void ShutdownModule() { }
}
