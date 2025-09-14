using System.Runtime.InteropServices;
using Retro.ReadOnlyParams.Annotations;
using UnrealSharp.Test.Interop;

namespace UnrealSharp.Test.Runner;

/// <summary>
/// Represents a read-only reference to an Unreal Engine automation test instance,
/// allowing interaction with native test execution workflows.
/// </summary>
/// <remarks>
/// This struct is used to provide a managed interface for working with native Unreal Engine
/// automation test instances. It enables enqueuing tasks that represent native latent commands
/// during automated test execution in Unreal Engine.
/// </remarks>
public readonly ref struct AutomationTestRef([ReadOnly] IntPtr nativeTest)
{
    /// <summary>
    /// Enqueues a task to be executed as a native latent command in the Unreal Engine automation test framework.
    /// </summary>
    /// <param name="task">The task to be executed. This represents the logic for a native latent command in Unreal Engine.</param>
    public void EnqueueNativeTask(Task task)
    {
        var taskHandle = GCHandle.Alloc(task);
        AutomationTestExporter.CallEnqueueLatentCommand(nativeTest, GCHandle.ToIntPtr(taskHandle));
    }
}
