using GameDataAccessTools.Editor.Interop;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace GameDataAccessTools.Editor;

[CustomLog]
public static partial class LogGameDataAccessToolsEditor;

public class FGameDataAccessToolsEditorModule : IModuleInterface
{
    public void StartupModule()
    {
        var actions = GameDataTypeManagedActions.Create();
        GameDataTypeCallbacksExporter.CallSetActions(ref actions);
    }

    public void ShutdownModule() { }
}
