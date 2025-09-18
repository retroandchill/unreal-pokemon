using JetBrains.Annotations;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

[UClass]
[UsedImplicitly]
public class UBagPocket : UInventoryContainer
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public FGameplayTag Tag { get; internal set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Display")]
    public FText Name { get; internal set; }

    protected override EInventoryValidation ValidateItem(FName item)
    {
        var handle = new FItemHandle(item);
        return handle.IsValid && Tag == handle.Entry.Pocket
            ? EInventoryValidation.Valid
            : EInventoryValidation.Invalid;
    }
}
