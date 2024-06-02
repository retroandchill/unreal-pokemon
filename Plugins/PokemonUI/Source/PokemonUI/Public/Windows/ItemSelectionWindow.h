// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"

#include "ItemSelectionWindow.generated.h"

struct FItem;
class IBag;
class UItemOption;

/**
 * Window for selecting an item from a particular pocket.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UItemSelectionWindow : public USelectableWidget {
    GENERATED_BODY()

  public:
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

  protected:
    /**
     * Slot an item entry into the window
     * @param Option The option that represents an item in the player's inventory
     * @param ItemIndex The index of the item being slotted
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotItem(UItemOption *Option, int32 ItemIndex);

  private:
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
     * The current pocket that exists within the bag that we're viewing.
     */
    FName CurrentPocket;

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
