// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PokemonActionOptions.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMenuOption.h"
#include "Handlers/BattleMenuHandler.h"
#include <functional>

void UPokemonActionOptions::NativePreConstruct() {
    Super::NativePreConstruct();
    ClearSelectableOptions();
    if (OptionClass != nullptr) {
        Algo::ForEach(MenuActions, std::bind_front(&UPokemonActionOptions::CreateMenuOption, this));
    }
}

const TScriptInterface<IBattler> &UPokemonActionOptions::GetCurrentBattler() const {
    return CurrentBattler;
}

void UPokemonActionOptions::SetBattler(const TScriptInterface<IBattler> &Battler) {
    CurrentBattler = Battler;
}

void UPokemonActionOptions::ExecuteCurrentHandler(UPokemonBattleScreen *Screen) {
    int32 CurrentIndex = GetIndex();
    check(MenuActions.IsValidIndex(CurrentIndex))
    MenuActions[CurrentIndex]->OnSelected(Screen);
}

void UPokemonActionOptions::CreateMenuOption(const UBattleMenuHandler *MenuHandler) {
    TObjectPtr<UBattleMenuOption> Option = WidgetTree->ConstructWidget(OptionClass);
    Option->InitFromHandler(MenuHandler);
    SlotOption(Option);
}