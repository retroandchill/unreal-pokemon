// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "DataManager.h"
#include "Field/FieldItemEffect.h"
#include "GiveItemToPokemon.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Screens/BagScreen.h"

#include "UseItem.generated.h"

/**
 *
 */
UCLASS(Abstract)
class POKEMONUI_API UUseItem : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()
  public:
    template <typename T>
        requires std::is_base_of_v<UUseItem, T>
    static T *ConstructUseItemNode(UBagScreen *Screen, FName Item, int32 Quantity) {
        auto Node = NewObject<T>();
        Node->SetWorldContext(Screen);
        Node->BagScreen = Screen;
        Node->ItemName = Item;
        Node->ItemQuantity = Quantity;
        return Node;
    }

  protected:
    template <typename T, typename... A>
        requires Pokemon::Items::FieldItem<T, A...>
    UE5Coro::TCoroutine<> UseItem(FForceLatentCoroutine = {}, A &&...Args) {
        if (auto &ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemName); co_await BagScreen->TryUseItem<T, A...>(ItemData, ItemQuantity, std::forward<A>(Args)...)) {
            ItemUsed.Broadcast();
        } else {
            ItemNotUsed.Broadcast();
        }
    }

  private:
    /**
     * Called when the item is given to the Pokémon
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemUsed;

    /**
     * Called when the player backs out (or the item cannot be given)
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemNotUsed;

    /**
     * The object used to obtain the state of the world to open the menu with
     */
    UPROPERTY()
    TObjectPtr<UBagScreen> BagScreen;

    /**
     * The new item to give
     */
    UPROPERTY()
    FName ItemName;

    /**
     * The new item to give
     */
    UPROPERTY()
    int32 ItemQuantity;
};
