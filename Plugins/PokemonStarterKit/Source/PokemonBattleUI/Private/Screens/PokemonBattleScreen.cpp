// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonBattleScreen.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Blueprint/WidgetTree.h"
#include "Windows/BattleMoveSelect.h"
#include "Windows/PokemonActionOptions.h"
#include <functional>

void UPokemonBattleScreen::NativeConstruct() {
    Super::NativeConstruct();
    ActionSelect->GetOnConfirm().AddDynamic(this, &UPokemonBattleScreen::OnActionSelected);
}

void UPokemonBattleScreen::SetBattle(const TScriptInterface<IBattle> &Battle) {
    CurrentBattle = Battle;
    Algo::ForEach(Panels, &UWidget::RemoveFromParent);
    Panels.Reset();
    Battle->ForEachSide(std::bind_front(&UPokemonBattleScreen::AddPanelsForSide, this));
}

void UPokemonBattleScreen::SelectAction(const TScriptInterface<IBattler> &Battler) {
    SelectingBattlers.Emplace(Battler);
    if (!SelectionIndex.IsSet()) {
        SelectionIndex.Emplace(0);
    }

    if (!ActionSelect->IsVisible()) {
        ActionSelect->SetBattler(Battler);
        ActionSelect->SetVisibility(ESlateVisibility::Visible);
        ActionSelect->SetIndex(0);
        ActionSelect->SetActive(true);
    }
}

UPokemonActionOptions * UPokemonBattleScreen::GetActionSelect() const {
    return ActionSelect;
}

void UPokemonBattleScreen::SelectMove(const TScriptInterface<IBattler> &Battler) {
    ActionSelect->SetActive(true);
    ActionSelect->SetVisibility(ESlateVisibility::Hidden);
    MoveSelect->SetBattler(Battler);
    MoveSelect->SetVisibility(ESlateVisibility::Visible);
    MoveSelect->SetIndex(0);
    MoveSelect->SetActive(true);
}

void UPokemonBattleScreen::AddPanelsForSide(int32 Index, const TScriptInterface<IBattleSide> &Side) {
    Algo::ForEach(Side->GetBattlers(), std::bind_front(&UPokemonBattleScreen::CreateBattlePanel, this, Index));
}

void UPokemonBattleScreen::CreateBattlePanel(int32 Side, const TScriptInterface<IBattler> &Battler) {
    check(PanelClasses.IsValidIndex(Side) && !PanelClasses[Side].IsNull())
    auto Panel = WidgetTree->ConstructWidget<UPokemonBattlePanel>(PanelClasses[Side].LoadSynchronous());
    Panel->SetBattler(Battler);
    SlotPanel(Panel, Side);
    Panels.Emplace(Panel);
}

void UPokemonBattleScreen::OnActionSelected(int32) {
    ActionSelect->ExecuteCurrentHandler(this);
}