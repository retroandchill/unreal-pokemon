using System.Runtime.InteropServices;
using JetBrains.Annotations;
using UnrealSharp.Core;
using UnrealSharp.Core.Marshallers;
using UnrealSharp.Engine;
using UnrealSharp.Test.Utils.Interop;

namespace UnrealSharp.Test.Utils;

[StructLayout(LayoutKind.Sequential)]
internal struct TestWorldWidget
{
    private readonly IntPtr _ptr;
    private readonly IntPtr _controlBlock;
}

[StructLayout(LayoutKind.Sequential)]
public struct NativeTestWorld
{
    internal IntPtr GameInstance { get; }

    internal IntPtr World { get; }

    private readonly TestWorldWidget _overlay;
    private readonly TestWorldWidget _gameLayerManager;

    internal NativeBool IsInPlay { get; }
}

/// <summary>
/// A test class designed to simulate and manipulate a game world environment during testing scenarios. Primarily used
/// for validating game world interactions, behaviors, and logic under controlled conditions.
/// </summary>
public sealed class TestWorld : IDisposable
{
    private NativeTestWorld _nativeWorld;
    private bool _disposed;

    /// <summary>
    /// Retrieves the game instance managed by the test world.
    /// </summary>
    public UGameInstance GameInstance => ObjectMarshaller<UGameInstance>.FromNative(_nativeWorld.GameInstance, 0);

    /// <summary>
    /// Retrieves the world managed by the test world object.
    /// </summary>
    public UWorld World => ObjectMarshaller<UWorld>.FromNative(_nativeWorld.World, 0);

    /// <summary>
    /// Checks whether the world is currently in play mode.
    /// </summary>
    public bool IsInPlay => _nativeWorld.IsInPlay.ToManagedBool();

    public TestWorld(bool beginPlay = false)
    {
        TestWorldExporter.CallInitializeTestWorld(ref _nativeWorld, beginPlay.ToNativeBool());
    }

    /// <inheritdoc />
    ~TestWorld()
    {
        Dispose();
    }

    /// <summary>
    /// Initiates the gameplay sequence for the managed world by setting up actors and marking the world as in play
    /// mode. This method should only be called once per world instance. Logs a warning if called multiple times for
    /// the same world.
    /// </summary>
    public void BeginPlay()
    {
        TestWorldExporter.CallBeginPlay(ref _nativeWorld);
    }

    /// <inheritdoc />
    public void Dispose()
    {
        if (_disposed)
        {
            return;
        }

        TestWorldExporter.CallFinalizeTestWorld(ref _nativeWorld);
        _disposed = true;

        GC.SuppressFinalize(this);
    }
}
