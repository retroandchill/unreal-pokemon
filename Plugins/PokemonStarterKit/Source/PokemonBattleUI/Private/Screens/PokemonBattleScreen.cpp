// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonBattleScreen.h"
#include "Algo/ForEach.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Moves/BattleMove.h"
#include "Blueprint/WidgetTree.h"
#include "Windows/BattleMoveSelect.h"
#include "Windows/PokemonActionOptions.h"
#include <functional>

void UPokemonBattleScreen::NativeConstruct() {
    Super::NativeConstruct();
    ActionSelect->GetOnConfirm().AddDynamic(this, &UPokemonBattleScreen::OnActionSelected);
    MoveSelect->GetOnMoveSelected().AddDynamic(this, &UPokemonBattleScreen::OnMoveSelected);
    MoveSelect->GetOnCancel().AddDynamic(this, &UPokemonBattleScreen::OnMoveCanceled);
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
        NextBattler(Battler);
    }
}

UPokemonActionOptions *UPokemonBattleScreen::GetActionSelect() const {
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

void UPokemonBattleScreen::Refresh() const {
    for (auto &Panel : Panels) {
        Panel->Refresh();
    }
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

void UPokemonBattleScreen::NextBattler(const TScriptInterface<IBattler> &Battler) {
    ActionSelect->SetBattler(Battler);
    ActionSelect->SetVisibility(ESlateVisibility::Visible);
    ActionSelect->SetIndex(0);
    ActionSelect->SetActive(true);
}

void UPokemonBattleScreen::OnActionSelected(int32) {
    ActionSelect->ExecuteCurrentHandler(this);
}

void UPokemonBattleScreen::OnMoveSelected(const TScriptInterface<IBattler> &Battler,
                                          const TScriptInterface<IBattleMove> &Move) {
    auto Targets = IBattleMove::Execute_GetAllPossibleTargets(Move.GetObject(), Battler);
    CurrentBattle->QueueAction(MakeUnique<FBattleActionUseMove>(Battler, Move, MoveTemp(Targets)));
    MoveSelect->SetActive(false);
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    check(SelectionIndex.IsSet())
    auto &SelIndex = SelectionIndex.GetValue();
    SelIndex++;
    if (SelectingBattlers.IsValidIndex(SelIndex)) {
        NextBattler(SelectingBattlers[SelIndex]);
    }
}

void UPokemonBattleScreen::OnMoveCanceled() {
    MoveSelect->SetActive(false);
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    ActionSelect->SetActive(true);
    ActionSelect->SetVisibility(ESlateVisibility::Visible);
}