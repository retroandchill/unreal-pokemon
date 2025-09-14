using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Test.Attributes;

[assembly: TestAssembly]

namespace UnrealSharp.Test.Sample;

public class UnrealSharpTestSampleModule : IModuleInterface
{
    public void StartupModule() { }

    public void ShutdownModule() { }
}
