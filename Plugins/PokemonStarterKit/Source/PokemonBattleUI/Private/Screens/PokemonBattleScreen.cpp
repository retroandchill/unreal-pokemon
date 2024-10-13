// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonBattleScreen.h"
#include "Algo/ForEach.h"
#include "Battle/Actions/BattleActionDoNothing.h"
#include "Battle/Actions/BattleActionSwitchPokemon.h"
#include "Battle/Actions/BattleActionUseItem.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Moves/BattleMove.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMoveSelect.h"
#include "Components/BattleSwitchPane.h"
#include "Components/ExpGainPane.h"
#include "Components/PokemonActionOptions.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Construct.h"
#include "Utilities/RPGMenuUtilities.h"
#include <functional>

DEFINE_INJECTABLE_DEPENDENCY(UPokemonBattleScreen)

UPokemonBattleScreen * UPokemonBattleScreen::AddPokemonBattleScreenToStack(const UObject *WorldContextObject) {
    return URPGMenuUtilities::InjectScreenToStack<UPokemonBattleScreen>(WorldContextObject).GetPtrOrNull();
}

void UPokemonBattleScreen::NativeConstruct() {
    Super::NativeConstruct();
    SwapToPanelDisplay();
}

const TScriptInterface<IBattle> &UPokemonBattleScreen::GetBattle() const {
    return CurrentBattle;
}

void UPokemonBattleScreen::SetBattle(const TScriptInterface<IBattle> &Battle) {
    CurrentBattle = Battle;
    Algo::ForEach(Panels, &UWidget::RemoveFromParent);
    Panels.Reset();

    int32 Index = 0;
    // clang-format off
    Battle->GetSides() |
        UE::Ranges::ForEach([this, &Index](const TScriptInterface<IBattleSide> &Side) {
            AddPanelsForSide(Index, Side);
            Index++;
        });
    // clang-format on

    BattleSwitchPane->SetBattle(CurrentBattle);
    ExpGainPane->SetBattle(CurrentBattle);
}

void UPokemonBattleScreen::ClearSelectingBattlers() {
    SelectingBattlers.Reset();
    SelectionIndex.Reset();
}

void UPokemonBattleScreen::SelectAction(const TScriptInterface<IBattler> &Battler) {
    bMandatorySwitch = false;
    SelectingBattlers.Emplace(Battler);
    if (!SelectionIndex.IsSet()) {
        SelectionIndex.Emplace(0);
    }

    if (!ActionSelect->IsVisible()) {
        NextBattler(Battler);
    }
}

void UPokemonBattleScreen::PromptMandatorySwitch(const TScriptInterface<IBattler> &Battler) {
    bMandatorySwitch = true;
    SelectingBattlers.Emplace(Battler);
    if (!SelectionIndex.IsSet()) {
        SelectionIndex.Emplace(0);
    }

    OnMandatorySwitch();
}

UPokemonActionOptions *UPokemonBattleScreen::GetActionSelect() const {
    return ActionSelect;
}

UBattleSwitchPane *UPokemonBattleScreen::GetBattleSwitchPane() const {
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

void UPokemonBattleScreen::AdvanceToNextSelection() {
    check(SelectionIndex.IsSet())
    auto &SelIndex = SelectionIndex.GetValue();
    SelIndex++;
    if (!bMandatorySwitch && SelectingBattlers.IsValidIndex(SelIndex)) {
        NextBattler(SelectingBattlers[SelIndex]);
    }
}

void UPokemonBattleScreen::OnMoveSelected(const TScriptInterface<IBattler> &Battler,
                                          const TScriptInterface<IBattleMove> &Move) {
    auto Targets = Move->GetAllPossibleTargets() | UE::Ranges::Construct<FTargetWithIndex>() | UE::Ranges::ToArray;
    CurrentBattle->QueueAction(MakeUnique<FBattleActionUseMove>(Battler, Move, MoveTemp(Targets)));
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    AdvanceToNextSelection();
}

void UPokemonBattleScreen::SkipRemainingActions() {
    ActionSelect->DeactivateWidget();
    ActionSelect->SetVisibility(ESlateVisibility::Hidden);
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    auto &SelIndex = SelectionIndex.GetValue();
    while (SelectingBattlers.IsValidIndex(SelIndex)) {
        CurrentBattle->QueueAction(MakeUnique<FBattleActionDoNothing>(SelectingBattlers[SelIndex]));
        SelIndex++;
    }
}

void UPokemonBattleScreen::OnMoveCanceled() {
    MoveSelect->DeactivateWidget();
    MoveSelect->SetVisibility(ESlateVisibility::Hidden);
    ActionSelect->ActivateWidget();
    ActionSelect->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonBattleScreen::OnSwitchSelected(const TScriptInterface<IBattler> &Battler,
                                            const TScriptInterface<IBattler> &Target) {
    CurrentBattle->QueueAction(MakeUnique<FBattleActionSwitchPokemon>(Battler, Target));
    if (!bMandatorySwitch || *SelectionIndex == SelectingBattlers.Num() - 1) {
        HideSwitchWindow();
    }
    AdvanceToNextSelection();
}

void UPokemonBattleScreen::OnUseItemOnPokemonSelected(FName ItemID, const TScriptInterface<IBattler> &User,
                                                      const TScriptInterface<IBattler> &Target) {
    CurrentBattle->QueueAction(
        MakeUnique<FBattleActionUseItem>(User, ItemID, FItemTarget(TWeakInterfacePtr<IBattler>(Target.GetObject()))));
    HideSwitchWindow();
    AdvanceToNextSelection();
}

void UPokemonBattleScreen::CompleteExpGain() {
    OnExpGainComplete.Broadcast();
}