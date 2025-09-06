// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UE5Coro.h"
#include "UObject/Interface.h"

#include "InventoryScreen.generated.h"

class IPokemon;
struct FItem;
class IInventoryScreen;

struct POKEMONCORE_API FSelectedItemHandle
{
    FSelectedItemHandle(const TScriptInterface<IInventoryScreen> &InScreen, const FItem &InItem, int32 InQuantity);

    const TScriptInterface<IInventoryScreen> &GetScreen() const
    {
        return Screen;
    }

    const FItem &GetItem() const
    {
        return Item;
    }

    int32 GetQuantity() const
    {
        return Quantity;
    }

  private:
    TScriptInterface<IInventoryScreen> Screen;
    const FItem &Item;
    int32 Quantity;
};

/**
 * Delegate for when an item is selected
 */
DECLARE_DELEGATE_OneParam(FOnItemSelected, TOptional<FSelectedItemHandle>);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FItemFilter, const FItem &, Item);

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONCORE_API UInventoryScreen : public UInterface
{
    GENERATED_BODY()
};

/**
 * Abstract declaration of the inventory screen used by the game
 */
class POKEMONCORE_API IInventoryScreen
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Apply the provided filter to the item list
     * @param ItemFilter The filter to apply to the list of items
     */
    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    virtual void ApplyItemFilter(const FItemFilter &ItemFilter) = 0;

    /**
     * Callback override for when an item is selected. When this is bound it will override the default behavior
     * for when an item is selected.
     * @return The callback for when an item is selected by the player
     */
    virtual UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> PromptItemSelection() = 0;

    /**
     * Toggle whether the player can select an item or not in the menu
     * @param bCanSelect Can the player select an item
     */
    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    virtual void ToggleItemSelection(bool bCanSelect) = 0;

    /**
     * Remove the screen from the stack
     */
    UFUNCTION(BlueprintCallable, Category = Navigation)
    virtual void RemoveFromStack() = 0;

    /**
     * Refresh the display of the scene
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual void RefreshScene() = 0;
};