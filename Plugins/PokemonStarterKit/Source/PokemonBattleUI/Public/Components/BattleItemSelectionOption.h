// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "BattleItemSelectionOption.generated.h"

struct FItem;
class UImage;
class UDisplayText;
/**
 * The individual item option to select in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleItemSelectionOption : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    /**
     * Set the currently displayed item.
     * @param Item The ID of the item.
     * @param Quantity The quantity of the item in question.
     */
    void SetItem(FName Item, int32 Quantity);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Item)
    void OnItemSet(const FItem &Item, int32 Quantity);

private:
    FName ItemID;
    int32 ItemQuantity;
};
