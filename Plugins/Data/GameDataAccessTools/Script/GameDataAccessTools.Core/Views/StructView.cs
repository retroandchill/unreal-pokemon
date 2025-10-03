using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Core.Views;

/// <summary>
/// The <c>StructView</c> struct provides a readonly, ref-struct view layer over a native
/// struct buffer, allowing operations on Unreal Engine-based marshalled structs in
/// a safe, managed context.
/// </summary>
/// <typeparam name="T">
/// The struct type that this view represents. Must be a struct that implements the
/// <see cref="MarshalledStruct{TStruct}"/> interface and meets the requirements for a ref struct.
/// </typeparam>
/// <remarks>
/// This struct simplifies the process of working with unmanaged memory in Unreal Engine
/// applications by offering utility functions for converting between managed and native
/// representations of the underlying struct type.
/// </remarks>
public readonly ref struct StructView<T>(IntPtr nativePtr)
    where T : struct, MarshalledStruct<T>, allows ref struct
{
    /// <summary>
    /// Represents a pointer or reference to a native resource or unmanaged code entity.
    /// The property provides direct access to the underlying native implementation
    /// or associated unmanaged resource, enabling interaction with low-level or
    /// platform-specific functionality. Proper handling of the pointer is necessary
    /// to avoid resource leaks or undefined behavior.
    /// </summary>
    public IntPtr NativePtr { get; } = nativePtr;

    /// <summary>
    /// Retrieves the native Unreal Engine script struct corresponding to the specified
    /// managed struct type <typeparamref name="T"/>.
    /// This property enables interop between managed and native data representations
    /// in a contextual, type-safe manner for Unreal Engine marshalled structs.
    /// </summary>
    /// <typeparam name="T">
    /// The type parameter representing the managed struct. This type must conform to the requirements
    /// for Unreal Engine marshalled structs and implement the <see cref="MarshalledStruct{TStruct}"/> interface.
    /// </typeparam>
    /// <remarks>
    /// The property leverages Unreal Engine's reflection and interop capabilities to find
    /// and map the native USTRUCT counterpart, facilitating safe access and manipulation
    /// of Unreal Engine data structures from managed code.
    /// </remarks>
    public static UScriptStruct ScriptStruct
    {
        get
        {
            var handle = FCSManagerExporter.CallFindManagedObject(T.GetNativeClassPtr());
            return GCHandleUtilities.GetObjectFromHandlePtr<UScriptStruct>(handle)!;
        }
    }

    /// <summary>
    /// Converts a native struct pointer to a <see cref="StructView{T}"/> instance, providing a managed view of the native data.
    /// </summary>
    /// <param name="nativeStruct">
    /// A pointer to the native struct that will be wrapped in the <see cref="StructView{T}"/>.
    /// </param>
    /// <returns>
    /// A new instance of <see cref="StructView{T}"/> that wraps the native struct pointer.
    /// </returns>
    public StructView<T> FromNative(IntPtr nativeStruct) => new(nativeStruct);

    /// <summary>
    /// Copies the data from the current <see cref="StructView{T}"/> instance to a specified native memory location.
    /// </summary>
    /// <param name="dest">
    /// A pointer to the destination native memory location where the data will be copied.
    /// </param>
    public void ToNative(IntPtr dest) => UScriptStructExporter.CallNativeCopy(T.GetNativeClassPtr(), NativePtr, dest);

    /// <summary>
    /// Copies the native struct data from <see cref="NativePtr"/> and converts it to a managed instance
    /// of type <typeparamref name="T"/>.
    /// </summary>
    /// <returns>
    /// A managed instance of type <typeparamref name="T"/> that represents the data from the associated native struct.
    /// </returns>
    public T Copy() => T.FromNative(NativePtr);

    /// <summary>
    /// Implicitly converts a <see cref="StructView{T}"/> instance to a managed instance of type <typeparamref name="T"/>.
    /// </summary>
    /// <param name="view">
    /// The <see cref="StructView{T}"/> instance to be converted to a managed struct of type <typeparamref name="T"/>.
    /// </param>
    /// <returns>
    /// A managed instance of type <typeparamref name="T"/> that represents the data contained in the <see cref="StructView{T}"/>.
    /// </returns>
    public static implicit operator T(StructView<T> view) => view.Copy();
}
