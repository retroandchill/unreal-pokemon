using GameDataAccessTools.Core.Views;
using UnrealSharp.Core;

namespace GameDataAccessTools.Core.Marshallers;

/// <summary>
/// Responsible for marshaling data from a native pointer into a managed <see cref="ArrayView{T}"/>.
/// </summary>
/// <typeparam name="T">
/// The type of data within the array view, constrained to types that support ref struct semantics.
/// </typeparam>
public class ArrayViewMarshaller<T>(Func<IntPtr, int, T> fromNative)
    where T : allows ref struct
{
    /// Converts a native buffer to an <see cref="ArrayView{T}"/>.
    /// <param name="buffer">A pointer to the start of the native buffer.</param>
    /// <param name="index">The index in the buffer to start reading from.</param>
    /// <returns>An <see cref="ArrayView{T}"/> instance containing the data from the native buffer.</returns>
    public ArrayView<T> FromNative(IntPtr buffer, int index)
    {
        unsafe
        {
            var unmanagedArray = (UnmanagedArray*)(buffer + index * sizeof(UnmanagedArray));
            return new ArrayView<T>(ref *unmanagedArray, fromNative);
        }
    }
}
