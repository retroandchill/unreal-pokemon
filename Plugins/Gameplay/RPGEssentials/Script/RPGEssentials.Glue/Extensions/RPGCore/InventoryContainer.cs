using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.RPGCore;

public partial class UInventoryContainer
{
    private MapReadOnlyMarshaller<FName, int>? _inventoryMarshaller;

    /// <summary>
    /// Represents a read-only collection of items stored in the inventory container.
    /// The collection maps item identifiers of type <see cref="FName"/> to integer quantities.
    /// </summary>
    /// <remarks>
    /// Provides an interface to read the inventory contents while ensuring the data remains immutable.
    /// </remarks>
    public TMapReadOnly<FName, int> Items
    {
        get
        {
            _inventoryMarshaller ??= new MapReadOnlyMarshaller<FName, int>(
                ItemSlots_NativeProperty,
                BlittableMarshaller<FName>.ToNative,
                BlittableMarshaller<FName>.FromNative,
                BlittableMarshaller<int>.ToNative,
                BlittableMarshaller<int>.FromNative
            );
            return _inventoryMarshaller.FromNative(IntPtr.Add(NativeObject, ItemSlots_Offset), 0);
        }
    }
}
