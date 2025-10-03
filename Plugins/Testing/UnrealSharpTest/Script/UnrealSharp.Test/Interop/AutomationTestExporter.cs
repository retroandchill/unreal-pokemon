using UnrealSharp.Binds;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Log;
using UnrealSharp.Test.Model;
using UnrealSharp.Test.Runner;

namespace UnrealSharp.Test.Interop;

/// <summary>
/// The <c>AutomationTestExporter</c> class provides interop functionality between managed code
/// and the Unreal Engine automation test framework.
/// </summary>
/// <remarks>
/// This class is marked as static, and all methods within are accessible without creating an instance.
/// It is primarily used to facilitate logging events, managing test cases, and executing automation tasks in Unreal Engine testing scenarios.
/// </remarks>
[NativeCallbacks]
public static unsafe partial class AutomationTestExporter
{
    private static readonly delegate* unmanaged<FName, char*, ref UnmanagedArray, ref UnmanagedArray, void> AddTestCase;
    private static readonly delegate* unmanaged<IntPtr, IntPtr, void> EnqueueLatentCommand;
    private static readonly delegate* unmanaged<char*, EAutomationEventType, void> LogEvent;
    private static readonly delegate* unmanaged<
        char*,
        ELogVerbosity,
        EAutomationExpectedMessageFlags,
        int,
        NativeBool,
        void> AddExpectedMessage;
}
