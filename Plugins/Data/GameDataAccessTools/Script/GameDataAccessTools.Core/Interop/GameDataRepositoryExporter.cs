using UnrealSharp;
using UnrealSharp.Binds;
using UnrealSharp.Core;

namespace GameDataAccessTools.Core.Interop;

/// <summary>
/// Provides functionality for exporting game data repositories, typically implemented using interop with native libraries.
/// </summary>
/// <remarks>
/// The <c>GameDataRepositoryExporter</c> class is designed as a static class to provide interoperability between managed
/// and unmanaged code. It is marked with the <c>[NativeCallbacks]</c> attribute to handle interactions with native libraries.
/// This class operates within the <c>GameDataAccessTools.Core.Interop</c> namespace and facilitates game data repository
/// exporting processes by exposing methods or configurations to bridge data between managed runtime and native game systems.
/// </remarks>
[NativeCallbacks]
public static unsafe partial class GameDataRepositoryExporter
{
    private static readonly delegate* unmanaged<IntPtr, IntPtr> GetEntries;
    private static readonly delegate* unmanaged<IntPtr, FName, IntPtr> GetEntryByID;
    private static readonly delegate* unmanaged<IntPtr, int, IntPtr> GetEntryByIndex;
}
