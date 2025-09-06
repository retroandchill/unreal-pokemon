using GameDataAccessTools.Core.Views;
using UnrealSharp;

namespace GameDataAccessTools.Core.Marshallers;

public static class StructViewMarshaller<T>
    where T : struct, MarshalledStruct<T>, allows ref struct
{
    public static StructView<T> FromNative(IntPtr buffer, int arrayIndex)
    {
        return new StructView<T>(buffer + arrayIndex * T.GetNativeDataSize());
    }
}
