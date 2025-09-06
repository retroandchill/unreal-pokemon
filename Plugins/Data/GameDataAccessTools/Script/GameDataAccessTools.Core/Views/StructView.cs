using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Core.Views;

public readonly ref struct StructView<T>(IntPtr nativePtr)
    where T : struct, MarshalledStruct<T>, allows ref struct
{
    public IntPtr NativePtr { get; } = nativePtr;

    public static UScriptStruct ScriptStruct
    {
        get
        {
            var handle = FCSManagerExporter.CallFindManagedObject(T.GetNativeClassPtr());
            return GCHandleUtilities.GetObjectFromHandlePtr<UScriptStruct>(handle)!;
        }
    }

    public StructView<T> FromNative(IntPtr nativeStruct) => new(nativeStruct);

    public void ToNative(IntPtr dest) =>
        UScriptStructExporter.CallNativeCopy(T.GetNativeClassPtr(), NativePtr, dest);

    public T Copy() => T.FromNative(NativePtr);

    public static implicit operator T(StructView<T> view) => view.Copy();
}
