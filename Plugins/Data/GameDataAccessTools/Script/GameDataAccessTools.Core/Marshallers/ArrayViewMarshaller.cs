using GameDataAccessTools.Core.Views;
using UnrealSharp.Core;

namespace GameDataAccessTools.Core.Marshallers;

public class ArrayViewMarshaller<T>(Func<IntPtr, int, T> fromNative)
    where T : allows ref struct
{
    public ArrayView<T> FromNative(IntPtr buffer, int index)
    {
        unsafe
        {
            var unmanagedArray = (UnmanagedArray*)(buffer + index * sizeof(UnmanagedArray));
            return new ArrayView<T>(ref *unmanagedArray, fromNative);
        }
    }
}
