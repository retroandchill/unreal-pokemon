using UnrealSharp.Binds;
using UnrealSharp.Core;

namespace GameDataAccessTools.Editor.Interop;

[NativeCallbacks]
public static unsafe partial class GameDataTypeCallbacksExporter
{
    private static readonly delegate* unmanaged<ref GameDataTypeManagedActions, void> SetActions;
    private static readonly delegate* unmanaged<
        ref UnmanagedArray,
        IntPtr,
        void> AddActionInformation;
}
