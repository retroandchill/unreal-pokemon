// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"

#include "BattleItemSelectPane.generated.h"

class UBattleItemSelectionOption;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSelected, FName, ItemID, int32, Quantity);

/**
 * Selection pane used to select items for usage in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleItemSelectPane : public USelectableWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Battle|Items", meta = (AutoCreateRefTerm = Usage))
    void SetBattleItemUse(UPARAM(meta = (Categories = "Battle.Items.BattleUse")) const FGameplayTag &Usage);

  private:
    void ItemSelected(FName Item, int32 Quantity) const;

    /**
     * The class for the instantiated items that are created
     */
    UPROPERTY(EditAnywhere, Category = "Content")
    TSubclassOf<UBattleItemSelectionOption> ItemOptionClass;

    /**
     * Called when an item is selected by this window.
     */
    UPROPERTY(BlueprintAssignable)
    FOnItemSelected OnItemSelected;
};
