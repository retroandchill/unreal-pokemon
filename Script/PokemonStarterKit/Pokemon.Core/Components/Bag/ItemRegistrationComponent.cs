using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

[UClass]
public class UItemRegistrationComponent : URPGComponent
{
    [UProperty]
    public TSet<FItemHandle> RegisteredItems { get; }
    
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory|Registration")]
    public bool IsRegistered(FItemHandle item) => RegisteredItems.Contains(item);
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory|Registration")]
    public void RegisterItem(FItemHandle item) => RegisteredItems.Add(item);
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory|Registration")]
    public void UnregisterItem(FItemHandle item) => RegisteredItems.Remove(item);
}