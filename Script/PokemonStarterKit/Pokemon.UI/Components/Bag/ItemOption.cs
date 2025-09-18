using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UItemOption : UCommonButtonBase
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public FItemHandle Item { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public int Quantity { get; private set; }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void SetItem(FItemHandle item, int quantity)
    {
        Item = item;
        Quantity = quantity;
        OnItemSet(item, quantity);
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Display")]
    protected virtual void OnItemSet(FItemHandle item, int quantity)
    {
        // No native implementation
    }
}
