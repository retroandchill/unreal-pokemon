// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonBattleScreen.h"
#include "Algo/ForEach.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Actions/BattleActionSwitchPokemon.h"
#include "Battle/Moves/BattleMove.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMoveSelect.h"
#include "Components/BattleSwitchPane.h"
#include "Components/ExpGainPane.h"
#include "Components/PokemonActionOptions.h"
#include <functional>

void UPokemonBattleScreen::NativeConstruct() {
    Super::NativeConstruct();
    SwapToPanelDisplay();
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

    BattleSwitchPane->SetBattle(CurrentBattle);
    ExpGainPane->SetBattle(CurrentBattle);
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

UBattleSwitchPane * UPokemonBattleScreen::GetBattleSwitchPane() const {
    return BattleSwitchPane;
}

UExpGainPane *UPokemonBattleScreen::GetExpGainPane() const {
    return ExpGainPane;
}

void UPokemonBattleScreen::SelectMove(const TScriptInterface<IBattler> &Battler) {
    ActionSelect->DeactivateWidget();
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

UPokemonBattlePanel *UPokemonBattleScreen::FindPanelForBattler(const TScriptInterface<IBattler> &Battler) const {
    auto Find = Panels.FindByPredicate(
        [&Battler](const UPokemonBattlePanel *Panel) { return Panel->GetCurrentBattler() == Battler; });
    return Find != nullptr ? *Find : nullptr;
}

void UPokemonBattleScreen::DisplayExpForGain(TArray<FExpGainInfo> &&GainInfos) {
    ExpGainPane->GainExp(MoveTemp(GainInfos));
    SwapToExpGainDisplay();
    PlayExpGainAnimation();
}

FDelegateHandle UPokemonBattleScreen::BindToExpGainComplete(FSimpleDelegate &&Callback) {
    return OnExpGainComplete.Add(MoveTemp(Callback));
}

void UPokemonBattleScreen::RemoveFromExpGainComplete(FDelegateHandle Handle) {
    OnExpGainComplete.Remove(Handle);
}

void UPokemonBattleScreen::FinishExpGainDisplay() {
    Algo::ForEach(Panels, &UPokemonBattlePanel::Refresh);
    SwapToPanelDisplay();
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

void UPokemonBattleScreen::AdvanceToNextSelection()
{
    check(SelectionIndex.IsSet())
    auto &SelIndex = SelectionIndex.GetValue();
    SelIndex++;
    if (SelectingBattlers.IsValidIndex(SelIndex)) {
        NextBattler(SelectingBattlers[SelIndex]);
    }
}

void UPokemonBattleScreen::OnMoveSelected(const TScriptInterface<IBattler> &Battler,
                                          const TScriptInterface<IBattleMove> &Move) {
    auto Targets = Move->GetAllPossibleTargets();
    CurrentBattle->QueueAction(MakeUnique<FBattleActionUseMove>(Battler, Move, MoveTemp(Targets)));
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    AdvanceToNextSelection();
}

void UPokemonBattleScreen::OnMoveCanceled() {
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    ActionSelect->ActivateWidget();
    ActionSelect->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonBattleScreen::OnSwitchSelected(const TScriptInterface<IBattler> &Battler, const TScriptInterface<IBattler> &Target) {
    CurrentBattle->QueueAction(MakeUnique<FBattleActionSwitchPokemon>(Battler, Target));
    HideSwitchWindow();
    AdvanceToNextSelection();
}

void UPokemonBattleScreen::CompleteExpGain() {
    OnExpGainComplete.Broadcast();
}