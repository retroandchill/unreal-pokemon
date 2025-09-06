using UnrealSharp.Core;

namespace GameDataAccessTools.Core.Marshallers;

public static class SpanMarshaller<T>
    where T : unmanaged
{
    public static ReadOnlySpan<T> FromNative(IntPtr buffer, int arrayIndex)
    {
        unsafe
        {
            var unmanagedArray = (UnmanagedArray*)(buffer + arrayIndex * sizeof(UnmanagedArray));
            return new ReadOnlySpan<T>((T*)unmanagedArray->Data, unmanagedArray->ArrayNum);
        }
    }
}
