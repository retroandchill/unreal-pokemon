using JetBrains.Annotations;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

[UClass]
public class UPocketsComponent : URPGComponent
{
    [UProperty]
    [UsedImplicitly]
    private TMap<FGameplayTag, UBagPocket> Pockets { get; }
    
    public IEnumerable<FPocketIdentifier> AllPockets => Pockets.Select(x => new FPocketIdentifier(x.Key, x.Value.Name));
    
    public int PocketCount => Pockets.Count;

    public ILookup<FGameplayTag, FItemSlot> AllItems
    {
        get
        {
            return Pockets.SelectMany(x => x.Value.Items.Select(y => new FItemSlot(y.Key, y.Value)).Select(z => (Tag: x.Key, Item: z)))
                .ToLookup(x => x.Tag, x => x.Item);
        }
    }
    
    [UFunction]
    [ExcludeFromExtensions]
    public virtual void Initialize(FBagInitParams initParams)
    {
        foreach (var (tag, name, maxSize) in initParams.Pockets)
        {
            var pocket = NewObject<UBagPocket>(this);
            pocket.MaxSlots = maxSize;
            pocket.MaxStackSize = initParams.MaxItemsPerSlot;
            pocket.Name = name;
            pocket.Tag = tag;
            Pockets.Add(tag, pocket);
        }
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int AddItem(FItemHandle item, int quantity = 1)
    {
        return GetPocket(item).AddItem(item, quantity);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int AddItemExact(FItemHandle item, int quantity = 1)
    {
        var pocket = GetPocket(item);
        var maxStackSize = pocket.MaxStackSize.Match(x => x, () => int.MaxValue);
        return maxStackSize - pocket.GetItemQuantity(item) >= quantity ? AddItem(item, quantity) : 0;
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int RemoveItem(FItemHandle item, int quantity = 1)
    {
        return GetPocket(item).RemoveItem(item, quantity);
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int RemoveItemExact(FItemHandle item, int quantity = 1)
    {
        var pocket = GetPocket(item);
        return pocket.GetItemQuantity(item) >= quantity ? RemoveItem(item, quantity) : 0;
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public bool ReplaceItem(FItemHandle item1, FItemHandle item2)
    {
        if (!item1.IsValid || !item2.IsValid || item1.Entry.Pocket != item2.Entry.Pocket) return false;
        
        return GetPocket(item1).ReplaceItem(item1, item2);
    }
    
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory")]
    public int GetItemQuantity(FItemHandle item)
    {
        return GetPocket(item).GetItemQuantity(item);
    }
    
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory")]
    public bool HasItem(FItemHandle item)
    {
        return GetPocket(item).HasItem(item);
    }

    [ExcludeFromExtensions]
    public UBagPocket GetPocket(FItemHandle item)
    {
        if (!item.IsValid)
        {
            throw new ArgumentException("Item is not valid");
        }

        return Pockets.TryGetValue(item.Entry.Pocket, out var pocket) ? pocket : throw new ArgumentException($"Invalid item pocket tag: {item.Entry.Pocket}");
    }
}