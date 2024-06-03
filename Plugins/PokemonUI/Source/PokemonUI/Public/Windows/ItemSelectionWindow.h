// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Mainpulation/CircularIterator.h"
#include "Windows/SelectableWidget.h"

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, const FItem&, Item, int32, Quantity);

/**
 * Delegate called when the player is not selecting any items.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoItemSelected);

/**
 * Window for selecting an item from a particular pocket.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UItemSelectionWindow : public USelectableWidget {
    GENERATED_BODY()

  public:
    explicit UItemSelectionWindow(const FObjectInitializer &ObjectInitializer);
    
    /**
     * Set the bag and starting pocket to view.
     * @param Bag The bag in question to open.
     * @param Pocket The pocket to view
     */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void SetBag(const TScriptInterface<IBag> &Bag, FName Pocket);

    /**
     * Get the window's currently selected item.
     * @return The item that is currently selected by the window
     */
    const FItem *GetCurrentItem() const;

    int32 GetItemCount_Implementation() const override;

    /**
     * Get the pocket changed dispatcher
     * @return Callback for when the pocket is changed.
     */
    FOnPocketChanged& GetOnPocketChanged();

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
    /**
     * Slot an item entry into the window
     * @param Option The option that represents an item in the player's inventory
     * @param ItemIndex The index of the item being slotted
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotItem(UItemOption *Option, int32 ItemIndex);

    void OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) override;
    void ReceiveMoveCursor(ECursorDirection Direction) override;

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

    /**
     * Iterator used to cycle through the pockets
     */
    TCircularIterator<FName> PocketIterator;

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

    /**
     * The list of option widgets in the window.
     */
    UPROPERTY()
    TArray<TObjectPtr<UItemOption>> Options;
};
