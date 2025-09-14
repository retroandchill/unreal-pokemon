using UnrealSharp.Binds;
using UnrealSharp.Core;

namespace UnrealSharp.Test.Interop;

/// <summary>
/// Provides static methods and callbacks intended for managing the communication
/// between managed C# code and native functionalities for testing purposes.
/// </summary>
/// <remarks>
/// This class facilitates interactions between managed and unmanaged code, including
/// exporting test cases, managing assemblies, and invoking specific unmanaged functions.
/// It is primarily intended to handle test discovery and lifecycle operations in UnrealSharp's
/// testing framework.
/// </remarks>
/// <threadsafety>
/// This static class is designed to handle unmanaged callbacks and is not inherently thread-safe.
/// Ensure appropriate synchronization when accessing shared native resources.
/// </threadsafety>
/// <example>
/// This class is used internally and should not be directly instantiated or called by user code.
/// Ensure that the appropriate methods are invoked through the orchestrated testing framework.
/// </example>
[NativeCallbacks]
public static unsafe partial class ManagedTestingExporter
{
    private static readonly delegate* unmanaged<ref ManagedTestingActions, void> SetManagedActions;
    private static readonly delegate* unmanaged<FName, IntPtr> FindUserAssembly;
    private static readonly delegate* unmanaged<
        ref UnmanagedArray,
        IntPtr,
        IntPtr,
        void> AddTestCase;
}
