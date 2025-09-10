using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace GameDataAccessTools.Core;

[CustomLog]
public static partial class LogGameDataAccessToolsCore;

[UsedImplicitly]
public class FManagedGameDataAccessTools : IModuleInterface
{
    public void StartupModule() { }

    public void ShutdownModule() { }
}
