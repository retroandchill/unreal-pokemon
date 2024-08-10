// "Unreal Pokémon" created by Retro & Chill.


#include "Components/BattleItemSelectPane.h"
#include "DataManager.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleItemSelectionOption.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"

void UBattleItemSelectPane::SetBattleItemUse(const FGameplayTag &Usage) {
    ClearSelectableOptions();
    auto &Bag = UPokemonSubsystem::GetInstance(this).GetBag();

    static auto &ItemTable = FDataManager::GetInstance().GetDataTable<FItem>();
    Bag->ForEachItem([this, &Usage](FName Item, int32 Quantity) {
        if (auto &ItemData = ItemTable.GetDataChecked(Item); ItemData.BattleUse == EBattleUse::NoBattleUse || !ItemData.BattleUsageCategories.HasTag(Usage)) {
            return;
        }
        
        auto Option = WidgetTree->ConstructWidget(ItemOptionClass);
        Option->SetItem(Item, Quantity);
        SlotOption(Option);
        Option->OnClicked().AddUObject(this, &UBattleItemSelectPane::ItemSelected, Item, Quantity);
    });
}

void UBattleItemSelectPane::ItemSelected(FName Item, int32 Quantity) const {
    OnItemSelected.Broadcast(Item, Quantity);
}