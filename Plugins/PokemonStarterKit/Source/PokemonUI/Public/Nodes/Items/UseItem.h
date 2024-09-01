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
class POKEMONUI_API UUseItem : public UBlueprintAsyncActionBase {
    GENERATED_BODY()
  public:
    template <typename T>
        requires std::is_base_of_v<UUseItem, T>
    static T *ConstructUseItemNode(UBagScreen *Screen, FName Item, int32 Quantity) {
        auto Node = NewObject<T>();
        Node->BagScreen = Screen;
        Node->ItemName = Item;
        Node->ItemQuantity = Quantity;
        return Node;
    }

  protected:
    template <typename T, typename... A>
        requires Pokemon::Items::FieldItem<T, A...>
    void UseItem(A &&...Args) {
        auto &ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemName);
        BagScreen->TryUseItem<T, A...>(
            ItemData, ItemQuantity,
            FOnItemEffectComplete::FDelegate::CreateUObject(this, &UUseItem::OnItemEffectComplete),
            Forward<A>(Args)...);
    }

  private:
    void OnItemEffectComplete(bool bSuccess);

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
