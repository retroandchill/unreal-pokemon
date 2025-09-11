using UnrealSharp.Binds;
using UnrealSharp.Core;

namespace GameDataAccessTools.Editor.Interop;

[NativeCallbacks]
public static unsafe partial class GameDataEntrySerializerExporter
{
    private static readonly delegate* unmanaged<
        ref UnmanagedArray,
        IntPtr,
        IntPtr,
        void> AddToStagingArray;
}
