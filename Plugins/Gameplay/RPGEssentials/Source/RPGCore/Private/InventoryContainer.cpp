// "Unreal Pokémon" created by Retro & Chill.

#include "InventoryContainer.h"

int32 UInventoryContainer::AddItem(const FName ItemID, const int32 Amount)
{
    if (Amount <= 0 || !CanAddItem(ItemID, Amount))
    {
        return 0;
    }

    const int32 CurrentQuantity = GetItemQuantity(ItemID);
    const int32 MaxStack = MaxStackSize.Get(INT_MAX);

    const int32 AmountToAdd = FMath::Min(MaxStack - CurrentQuantity, Amount);

    if (AmountToAdd <= 0)
    {
        return 0;
    }

    ItemSlots.Add(ItemID, CurrentQuantity + AmountToAdd);
    return AmountToAdd;
}

int32 UInventoryContainer::RemoveItem(const FName ItemID, const int32 Amount)
{
    if (Amount <= 0)
    {
        return 0;
    }

    int32 *Quantity = ItemSlots.Find(ItemID);
    if (Quantity == nullptr)
    {
        return 0;
    }

    const int32 AmountToRemove = FMath::Min(*Quantity, Amount);
    *Quantity -= AmountToRemove;

    if (*Quantity <= 0)
    {
        ItemSlots.Remove(ItemID);
    }

    return AmountToRemove;
}

bool UInventoryContainer::ReplaceItem(const FName OldItem, const FName NewItem)
{
    if (!ValidateItem(OldItem) || !ValidateItem(NewItem))
        return false;
    if (OldItem == NewItem)
        return true;
    if (!HasItem(OldItem) || HasItem(NewItem))
        return false;

    const int32 Amount = GetItemQuantity(OldItem);
    ItemSlots.Remove(OldItem);
    ItemSlots.Add(NewItem, Amount);
    return true;
}

void UInventoryContainer::Clear()
{
    ItemSlots.Empty();
}

int32 UInventoryContainer::GetItemQuantity(const FName ItemID) const
{
    return ItemSlots.FindRef(ItemID, 0);
}

bool UInventoryContainer::CanAddItem(const FName ItemID, const int32 Amount) const
{
    if (!NativeCanAddItem(ItemID, Amount))
    {
        return false;
    }

    return K2_CanAddItem(ItemID, Amount) == EInventoryValidation::Valid;
}

bool UInventoryContainer::ValidateItem(const FName ItemID) const
{
    if (!NativeValidateItem(ItemID))
    {
        return false;
    }

    return K2_ValidateItem(ItemID) == EInventoryValidation::Valid;
}

bool UInventoryContainer::HasItem(const FName ItemID) const
{
    return ItemSlots.Contains(ItemID);
}

bool UInventoryContainer::IsEmpty() const
{
    return ItemSlots.IsEmpty();
}

bool UInventoryContainer::NativeCanAddItem(const FName ItemID, const int32 Amount) const
{
    if (!ValidateItem(ItemID))
    {
        return false;
    }

    if (MaxSlots.IsSet() && HasItem(ItemID) && ItemSlots.Num() >= MaxSlots.GetValue())
    {
        return false;
    }

    return !MaxStackSize.IsSet() || *MaxStackSize < GetItemQuantity(ItemID);
}

bool UInventoryContainer::NativeValidateItem(const FName ItemID) const
{
    return !ItemID.IsNone();
}