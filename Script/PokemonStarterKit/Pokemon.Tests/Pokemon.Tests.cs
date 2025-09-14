using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Test.Attributes;

[assembly: TestAssembly]

namespace Pokemon.Tests;

public class FPokemonTestsModule : IModuleInterface
{
    public void StartupModule() { }

    public void ShutdownModule() { }
}
