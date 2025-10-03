using UnrealSharp.Core;

namespace GameDataAccessTools.Core.Marshallers;

/// <summary>
/// Provides functionality to marshal unmanaged native data into a read-only span for managed code.
/// </summary>
/// <typeparam name="T">
/// The unmanaged type of data to be marshaled. This type must satisfy the 'unmanaged' constraint.
/// </typeparam>
public static class SpanMarshaller<T>
    where T : unmanaged
{
    /// <summary>
    /// Marshals data from a native pointer to a read-only span of type <typeparamref name="T"/>.
    /// </summary>
    /// <typeparam name="T">The unmanaged type of data to be marshaled. This type must satisfy the 'unmanaged' constraint.</typeparam>
    /// <param name="buffer">The pointer to the native memory buffer holding the data.</param>
    /// <param name="arrayIndex">The index of the desired element within the unmanaged array structure.</param>
    /// <returns>A read-only span of type <typeparamref name="T"/> representing the marshaled data.</returns>
    public static ReadOnlySpan<T> FromNative(IntPtr buffer, int arrayIndex)
    {
        unsafe
        {
            var unmanagedArray = (UnmanagedArray*)(buffer + arrayIndex * sizeof(UnmanagedArray));
            return new ReadOnlySpan<T>((T*)unmanagedArray->Data, unmanagedArray->ArrayNum);
        }
    }
}
