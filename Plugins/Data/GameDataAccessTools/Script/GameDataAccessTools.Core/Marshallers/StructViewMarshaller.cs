using GameDataAccessTools.Core.Views;
using UnrealSharp;

namespace GameDataAccessTools.Core.Marshallers;

/// <summary>
/// The StructViewMarshaller class provides methods for working with managed views of native struct representations.
/// It is intended to handle marshalling operations by creating views of unmanaged memory mapped to specific struct types.
/// </summary>
/// <typeparam name="T">
/// A struct type that implements the MarshalledStruct interface and satisfies the constraints
/// of being a struct and capable of behaving as a ref struct.
/// </typeparam>
/// <remarks>
/// This static class helps in creating instances of <see cref="StructView{T}"/> from native pointers
/// and offers a standardized way of manipulating structures stored in memory.
/// </remarks>
public static class StructViewMarshaller<T>
    where T : struct, MarshalledStruct<T>, allows ref struct
{
    /// <summary>
    /// Converts a native memory buffer to a strongly typed <see cref="StructView{T}"/> starting at the specified index in the array.
    /// </summary>
    /// <typeparam name="T">The type of the structure being marshalled. Must implement the <see cref="StructView{T}"/> interface and be a ref struct.</typeparam>
    /// <param name="buffer">The pointer to the native memory buffer.</param>
    /// <param name="arrayIndex">The index of the array where the view of the structure will start.</param>
    /// <returns>A <see cref="StructView{T}"/> instance that represents the structure starting from the given array index in the native memory buffer.</returns>
    public static StructView<T> FromNative(IntPtr buffer, int arrayIndex)
    {
        return StructView<T>.FromNative(buffer + arrayIndex * T.GetNativeDataSize());
    }
}
