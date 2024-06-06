﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Screens/InventoryScreen.h"

#include "ChooseItemFromBag.generated.h"

class UBagScreen;
class IInventoryScreen;
struct FItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemSelectedDynamic, const TScriptInterface<IInventoryScreen> &, Screen,
                                               const FItem&, Item, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemCancel);

/**
 * Choose and item from the bag and process the item accordingly
 */
UCLASS(meta = (HideThen, HasDedicatedAsyncNode))
class POKEMONUI_API UChooseItemFromBag : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Open the menu to select a Pokémon from the party
     * @param WorldContextObject The object used to obtain the state of the world
     * @param ScreenClass The class used to display the selection screen
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Selection")
    static UChooseItemFromBag *ChooseItemFromBag(const UObject *WorldContextObject, TSubclassOf<UBagScreen> ScreenClass);

    void Activate() override;
    
private:
    /**
     * Function called to execute the on selected pin
     * @param Screen The screen displayed
     * @param Item The item selected
     * @param Quantity The quantity of items held
     */
    UFUNCTION()
    void ExecuteOnSelected(const TScriptInterface<IInventoryScreen> &Screen, const FItem& Item, int32 Quantity) const;

    /**
     * Function called to execute the on cancelled pin
     */
    UFUNCTION()
    void ExecuteOnCanceled();
    
    /**
     * Called when the player selects an item
     */
    UPROPERTY(BlueprintAssignable)
    FItemSelectedDynamic OnSelected;

    /**
     * Called when the player cancels selection
     */
    UPROPERTY(BlueprintAssignable)
    FOnItemCancel OnCanceled;
    
    /**
     * The object used to obtain the state of the world to open the menu with
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContextObject;

    /**
     * The class used to display the message to the screen
     */
    UPROPERTY()
    TSubclassOf<UBagScreen> ScreenClass;

};
