// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "InventoryContainer.generated.h"

UENUM(BlueprintType)
enum class EInventoryValidation : uint8
{
    Valid,
    Invalid
};


/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class RPGCORE_API UInventoryContainer : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 AddItem(FName ItemID, int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 RemoveItem(FName ItemID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool ReplaceItem(FName OldItem, FName NewItem);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void Clear();

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemQuantity(FName ItemID) const;
    
    bool CanAddItem(FName ItemID, int32 Amount) const;
    bool ValidateItem(FName ItemID) const;
    
    TOptional<int32> GetMaxSlots() const
    {
        return MaxSlots;
    }

    TOptional<int32> GetMaxStackSize() const
    {
        return MaxStackSize;   
    }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(FName ItemID) const;
    
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool IsEmpty() const;
    
    const TMap<FName, int32> &GetItems() const
    {
        return ItemSlots;
    }

protected:
    void SetMaxSlots(const TOptional<int32> &InMaxSlots)
    {
        MaxSlots = InMaxSlots;
    }

    void SetMaxStackSize(const TOptional<int32> &InMaxStackSize)
    {
        MaxStackSize = InMaxStackSize;
    }

    virtual bool NativeCanAddItem(FName ItemID, int32 Amount) const;

    virtual bool NativeValidateItem(FName ItemID) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory", meta = (ScriptName = "CanAddItem"))
    EInventoryValidation K2_CanAddItem(FName ItemID, int32 Amount) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory", meta = (ScriptName = "ValidateItem"))
    EInventoryValidation K2_ValidateItem(FName ItemID) const;

private:
    UPROPERTY(SaveGame)
    TMap<FName, int32> ItemSlots;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TOptional<int32> MaxSlots;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TOptional<int32> MaxStackSize;

};
