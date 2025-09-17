using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.RPGCore;

public partial class UInventoryContainer
{
    
    private MapReadOnlyMarshaller<FName, int>? _inventoryMarshaller;

    public TMapReadOnly<FName, int> Items
    {
        get
        {
            _inventoryMarshaller ??= new MapReadOnlyMarshaller<FName, int>(ItemSlots_NativeProperty, BlittableMarshaller<FName>.ToNative, BlittableMarshaller<FName>.FromNative, BlittableMarshaller<int>.ToNative, BlittableMarshaller<int>.FromNative);
            return _inventoryMarshaller.FromNative(IntPtr.Add(NativeObject, ItemSlots_Offset), 0);
        }
    }
}