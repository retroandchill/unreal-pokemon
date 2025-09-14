using UnrealSharp.Binds;

namespace UnrealSharp.Test.Utils.Interop;

[NativeCallbacks]
public static unsafe partial class ScopedInstancesExporter
{
    private static readonly delegate* unmanaged<IntPtr, void> RemovePlayer;
}
