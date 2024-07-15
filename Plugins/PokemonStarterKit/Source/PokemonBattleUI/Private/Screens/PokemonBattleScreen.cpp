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
    ActionSelect->GetOnConfirm().AddUniqueDynamic(this, &UPokemonBattleScreen::OnActionSelected);
    MoveSelect->GetOnMoveSelected().AddUniqueDynamic(this, &UPokemonBattleScreen::OnMoveSelected);
    MoveSelect->GetOnCancel().AddUniqueDynamic(this, &UPokemonBattleScreen::OnMoveCanceled);
}

void UPokemonBattleScreen::SetBattle(const TScriptInterface<IBattle> &Battle) {
    CurrentBattle = Battle;
    Algo::ForEach(Panels, &UWidget::RemoveFromParent);
    Panels.Reset();

    int32 Index = 0;
    std::ranges::for_each(Battle->GetSides(), [this, &Index](const TScriptInterface<IBattleSide> &Side) {
        AddPanelsForSide(Index, Side);
        Index++;
    });
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
    ActionSelect->ActivateWidget();
    ActionSelect->SetVisibility(ESlateVisibility::Hidden);
    MoveSelect->SetBattler(Battler);
    MoveSelect->SetVisibility(ESlateVisibility::Visible);
    MoveSelect->SetIndex(0);
    MoveSelect->ActivateWidget();
}

void UPokemonBattleScreen::Refresh() const {
    for (auto &Panel : Panels) {
        Panel->Refresh();
    }
}

UPokemonBattlePanel * UPokemonBattleScreen::FindPanelForBattler(const TScriptInterface<IBattler> &Battler) const {
    auto Find = Panels.FindByPredicate([&Battler](const UPokemonBattlePanel* Panel)
        { return Panel->GetCurrentBattler() == Battler; });
    return Find != nullptr ? *Find : nullptr;
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
    ActionSelect->ActivateWidget();
}

void UPokemonBattleScreen::OnActionSelected(int32) {
    ActionSelect->ExecuteCurrentHandler(this);
}

void UPokemonBattleScreen::OnMoveSelected(const TScriptInterface<IBattler> &Battler,
                                          const TScriptInterface<IBattleMove> &Move) {
    auto Targets = Move->GetAllPossibleTargets();
    CurrentBattle->QueueAction(MakeUnique<FBattleActionUseMove>(Battler, Move, MoveTemp(Targets)));
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    check(SelectionIndex.IsSet())
    auto &SelIndex = SelectionIndex.GetValue();
    SelIndex++;
    if (SelectingBattlers.IsValidIndex(SelIndex)) {
        NextBattler(SelectingBattlers[SelIndex]);
    }
}

void UPokemonBattleScreen::OnMoveCanceled() {
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    ActionSelect->ActivateWidget();
    ActionSelect->SetVisibility(ESlateVisibility::Visible);
}