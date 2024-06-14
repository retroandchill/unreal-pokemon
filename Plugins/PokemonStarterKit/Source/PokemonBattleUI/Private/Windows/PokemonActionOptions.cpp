// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PokemonActionOptions.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMenuOption.h"
#include "Handlers/BattleMenuHandler.h"
#include "Mainpulation/RangeHelpers.h"

#include <functional>

TSharedRef<SWidget> UPokemonActionOptions::RebuildWidget() {
    auto Ret = Super::RebuildWidget();

    Algo::ForEach(Options, &UWidget::RemoveFromParent);
    Options.Reset();
    if (OptionClass != nullptr) {
        Algo::ForEach(MenuActions, std::bind_front(&UPokemonActionOptions::CreateMenuOption, this));
    }
    
    return Ret;
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

int32 UPokemonActionOptions::GetItemCount_Implementation() const {
    return Options.Num();
}

void UPokemonActionOptions::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    if (Options.IsValidIndex(OldIndex)) {
        Options[OldIndex]->OnUnselected();
    }

    if (Options.IsValidIndex(NewIndex)) {
        Options[NewIndex]->OnSelected();
    }
}

void UPokemonActionOptions::CreateMenuOption(const UBattleMenuHandler *MenuHandler) {
    TObjectPtr<UBattleMenuOption> Option = WidgetTree->ConstructWidget(OptionClass);
    Option->InitFromHandler(MenuHandler);
    SlotOption(Option);
    Option->SetOptionIndex(Options.Num());
    Option->GetOnOptionClicked().AddDynamic(this, &UPokemonActionOptions::ProcessClickedButton);
    Option->GetOnOptionHovered().AddDynamic(this, &UPokemonActionOptions::ProcessHoveredButton);
    Options.Emplace(Option);
}