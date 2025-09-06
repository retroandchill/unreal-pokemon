// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"
#include "Screens/InventoryScreen.h"

#include "ChooseItemFromBag.generated.h"

class UBagScreen;
struct FItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemSelectedDynamic, const TScriptInterface<IInventoryScreen> &, Screen,
                                               const FItem &, Item, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemCancel);

/**
 * Choose and item from the bag and process the item accordingly
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UChooseItemFromBag : public UBlueprintCoroutineActionBase
{
    GENERATED_BODY()

  public:
    /**
     * Open the menu to select a Pokémon from the party
     * @param WorldContextObject The object used to obtain the state of the world
     * @param ScreenClass The class used to display the selection screen
     * @param ItemFilter The filter used for the item list
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Selection")
    static UChooseItemFromBag *ChooseItemFromBag(const UObject *WorldContextObject, const FItemFilter &ItemFilter);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine = {}) override;

  private:
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
     * The filter used for the item list
     */
    UPROPERTY()
    FItemFilter ItemFilter;
};
