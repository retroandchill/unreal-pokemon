using System.Runtime.InteropServices;
using JetBrains.Annotations;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Editor.Interop;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct GameDataTypeManagedActions
{
    public required delegate* unmanaged<IntPtr, NativeBool> IsValidGameDataHandle { get; init; }

    public required delegate* unmanaged<IntPtr, UnmanagedArray*, void> GetDataHandleActions { get; init; }

    public static GameDataTypeManagedActions Create()
    {
        return new GameDataTypeManagedActions
        {
            IsValidGameDataHandle = &GameDataTypeManagedCallbacks.IsValidGameDataHandle,
            GetDataHandleActions = &GameDataTypeManagedCallbacks.GetDataHandleActions,
        };
    }
}

public static unsafe class GameDataTypeManagedCallbacks
{
    [UnmanagedCallersOnly]
    public static NativeBool IsValidGameDataHandle(IntPtr action)
    {
        var handleType = GCHandleUtilities.GetObjectFromHandlePtr<Type>(action);
        return handleType?.IsGameDataHandle().ToNativeBool() ?? NativeBool.False;
    }

    [UnmanagedCallersOnly]
    public static void GetDataHandleActions(IntPtr action, UnmanagedArray* outArray)
    {
        var handleType = GCHandleUtilities.GetObjectFromHandlePtr<Type>(action);
        if (handleType?.IsGameDataHandle() is not true)
        {
            return;
        }

        var nativeStruct = FDataHandleEntry.GetNativeClassPtr();
        var structBuffer = stackalloc byte[FDataHandleEntry.GetNativeDataSize()];
        try
        {
            foreach (var entry in handleType.GetDataHandleEntries())
            {
                entry.ToNative((IntPtr)structBuffer);
                GameDataTypeCallbacksExporter.CallAddActionInformation(ref *outArray, (IntPtr)structBuffer);
            }
        }
        catch (Exception e)
        {
            LogGameDataAccessToolsEditor.LogError($"Unexpected error when trying to get entries: {e.Message}");
        }
        finally
        {
            UScriptStructExporter.CallNativeDestroy(nativeStruct, (IntPtr)structBuffer);
        }
    }
}
