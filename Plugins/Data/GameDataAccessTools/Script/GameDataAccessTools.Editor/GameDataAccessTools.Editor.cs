using GameDataAccessTools.Editor.Interop;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace GameDataAccessTools.Editor;

/// <summary>
/// Provides custom logging functionality specific to the Game Data Access Tools Editor module.
/// </summary>
/// <remarks>
/// This class is a partial static class marked with the <see cref="UnrealSharp.Log.CustomLogAttribute"/> attribute,
/// allowing structured and reusable logging capabilities for managing game data access tools editor workflows.
/// The logging utility is leveraged in scenarios such as handling game data actions,
/// exporting callback information, and error handling within data repository workflows.
/// </remarks>
[CustomLog]
public static partial class LogGameDataAccessToolsEditor;

public class FGameDataAccessToolsEditorModule : IModuleInterface
{
    /// <inheritdoc />
    public void StartupModule()
    {
        var actions = GameDataTypeManagedActions.Create();
        GameDataTypeCallbacksExporter.CallSetActions(ref actions);
    }

    /// <inheritdoc />
    public void ShutdownModule() { }
}
