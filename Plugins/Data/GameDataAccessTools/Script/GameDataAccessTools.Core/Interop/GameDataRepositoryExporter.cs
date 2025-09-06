using UnrealSharp;
using UnrealSharp.Binds;

namespace GameDataAccessTools.Core.Interop;

[NativeCallbacks]
public static unsafe partial class GameDataRepositoryExporter
{
    private static readonly delegate* unmanaged<IntPtr, IntPtr> GetEntries;
    private static readonly delegate* unmanaged<IntPtr, FName, IntPtr> GetEntryByID;
    private static readonly delegate* unmanaged<IntPtr, int, IntPtr> GetEntryByIndex;
}
