// "Unreal Pokémon" created by Retro & Chill.


#include "Components/BattleItemSelectPane.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleItemSelectionOption.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"

void UBattleItemSelectPane::SetBattleItemUse(const FGameplayTag &Usage) {
    ClearSelectableOptions();
    auto &Bag = UPokemonSubsystem::GetInstance(this).GetBag();

    
    Bag->ForEachItem([this](FName Item, int32 Quantity) {
        auto Option = WidgetTree->ConstructWidget(ItemOptionClass);
        Option->SetItem(Item, Quantity);
        AddOptionToWidget(Option);
    });
}