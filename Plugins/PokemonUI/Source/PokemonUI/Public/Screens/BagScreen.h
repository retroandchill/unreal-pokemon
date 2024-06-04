// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "InventoryScreen.h"
#include "Screens/Screen.h"

#include "BagScreen.generated.h"

class UCommandWindow;
struct FItem;
class UPocketWindow;
class UItemInfoWindow;
class UItemSelectionWindow;

/**
 * The screen that contains the basic outline of the player's bag
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UBagScreen : public UScreen, public IInventoryScreen {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;

public:
    FOnItemSelected& GetOnItemSelected() final;

    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    void ToggleItemSelection(bool bCanSelect) final;

private:
    /**
     * Called when an item is selected by the player
     * @param Item The item that has been selected.
     * @param Quantity The quantity that has been selected
     */
    UFUNCTION()
    void SelectItem(const FItem& Item, int32 Quantity);

    /**
     * Called then the player cancels in the item command window
     */
    UFUNCTION()
    void OnItemCommandCanceled();
    
    /**
     * The window that displays the pockets
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPocketWindow> PocketWindow;

    /**
     * The window that displays the item list
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UItemSelectionWindow> ItemSelectionWindow;

    /**
     * The window that displays the item information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UItemInfoWindow> ItemInfoWindow;

    /**
     * The window that contains the command to call on an item
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommandWindow> CommandWindow;

    UPROPERTY()
    FOnItemSelected OnItemSelected;
};
