// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Components/SelectableWidget.h"
#include "Screens/InventoryScreen.h"

#include "ItemSelectionWindow.generated.h"

class IBag;
class UItemOption;

/**
 * Delegate called when the player changes pockets in the bag screen
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPocketChanged, FName, Pocket);

/**
 * Delegate called when the player selects a new item in the bag screen.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, const FItem &, Item, int32, Quantity);

/**
 * Delegate called when the player is not selecting any items.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoItemSelected);

/**
 * Window for selecting an item from a particular pocket.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UItemSelectionWindow : public USelectableWidget
{
    GENERATED_BODY()

  public:
    explicit UItemSelectionWindow(const FObjectInitializer &ObjectInitializer);

    /**
     * Set the bag and starting pocket to view.
     * @param Bag The bag in question to open.
     */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void SetBag(const TScriptInterface<IBag> &Bag);

    void SetPocket(FName Pocket);

    /**
     * Get the window's currently selected item.
     * @return The item that is currently selected by the window
     */
    const FItem *GetCurrentItem() const;

    int32 GetItemQuantity() const;

    void RefreshWindow();

    /**
     * Apply the provided filter to the item list
     * @param ItemFilter The filter to apply to the list of items
     */
    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    void ApplyItemFilter(const FItemFilter &ItemFilter);

    /**
     * Get the item selected callback
     * @return Callback for when an item is actually selected
     */
    FOnItemChanged &GetOnItemSelected();

    /**
     * Get the pocket changed dispatcher
     * @return Callback for when the pocket is changed.
     */
    FOnPocketChanged &GetOnPocketChanged();

    /**
     * Get the item changed dispatcher
     * @return Callback for when the item is changed.
     */
    FOnItemChanged &GetOnItemChanged();

    /**
     * Get the item changed dispatcher for no item
     * @return Callback for when no item is selected.
     */
    FOnNoItemSelected &GetOnNoItemSelected();

  protected:
    void NativeOnSelectionChange(int32 OldIndex, int32 NewIndex) override;
    void NativeProcessConfirm(int32 CurrentIndex) override;

  private:
    /**
     * Update the pocket that is being displayed
     */
    void UpdatePocket();

    /**
     * Add an item to the window with the given name and quantity
     * @param ItemName The identifier of the item
     * @param Quantity The number possessed
     */
    void AddItemToWindow(FName ItemName, int32 Quantity);

    /**
     * A reference to the bag that this window is displaying the items of
     */
    UPROPERTY()
    TScriptInterface<IBag> CurrentBag;

    FName CurrentPocket;

    /**
     * Filter to apply to the item list
     */
    FItemFilter ItemListFilter;

    /**
     * Callback for when an item is actually selected
     */
    UPROPERTY(BlueprintAssignable, Category = "Events|Inventory")
    FOnItemChanged OnItemSelected;

    /**
     * Callback for when the pocket is changed.
     */
    UPROPERTY(BlueprintAssignable, Category = "Events|Inventory")
    FOnPocketChanged OnPocketChanged;

    /**
     * Callback for when the selected item changed.
     */
    UPROPERTY(BlueprintAssignable, Category = "Events|Inventory")
    FOnItemChanged OnItemChanged;

    /**
     * Callback for when no item is selected.
     */
    UPROPERTY(BlueprintAssignable, Category = "Events|Inventory")
    FOnNoItemSelected OnNoItemSelected;

    /**
     * Class that is used to spawn options into the window
     */
    UPROPERTY(EditAnywhere, Category = "Display")
    TSubclassOf<UItemOption> ItemEntryClass;
};
