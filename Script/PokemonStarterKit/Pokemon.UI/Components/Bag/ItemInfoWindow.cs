using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.PokemonData;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UItemInfoWindow : UCommonUserWidget
{
    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void SetItem(FItemHandle item, int quantity)
    {
        // No native implementation
    }

    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void ClearItem()
    {
        // No native implementation
    }
}
