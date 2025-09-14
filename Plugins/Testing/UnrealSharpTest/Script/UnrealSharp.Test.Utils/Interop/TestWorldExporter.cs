using UnrealSharp.Binds;
using UnrealSharp.Core;

namespace UnrealSharp.Test.Utils.Interop;

[NativeCallbacks]
public static unsafe partial class TestWorldExporter
{
    private static readonly delegate* unmanaged<
        ref NativeTestWorld,
        NativeBool,
        void> InitializeTestWorld;
    private static readonly delegate* unmanaged<ref NativeTestWorld, void> FinalizeTestWorld;
    private static readonly delegate* unmanaged<ref NativeTestWorld, void> BeginPlay;
}
