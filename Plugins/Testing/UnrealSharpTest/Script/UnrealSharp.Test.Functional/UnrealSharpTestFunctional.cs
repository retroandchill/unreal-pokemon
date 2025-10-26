using JetBrains.Annotations;
using UnrealSharp.Engine.Core.Modules;

namespace UnrealSharp.Test.Functional;

/// <summary>
/// Represents the UnrealSharp test functional class implementing the IModuleInterface.
/// This class is used for defining startup and shutdown behaviors within the functional module.
/// </summary>
public class FUnrealSharpTestFunctional : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule() { }

    /// <inheritdoc />
    public void ShutdownModule() { }
}
