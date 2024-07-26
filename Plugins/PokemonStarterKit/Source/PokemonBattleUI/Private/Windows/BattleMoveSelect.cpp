// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/BattleMoveSelect.h"
#include "Algo/ForEach.h"
#include "Battle/Battlers/Battler.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMovePanel.h"
#include <functional>

void UBattleMoveSelect::SetBattler(const TScriptInterface<IBattler> &NewBattler) {
    CurrentBattler = NewBattler;
    ClearSelectableOptions();
    Algo::ForEach(NewBattler->GetMoves(), std::bind_front(&UBattleMoveSelect::CreateMovePanel, this));
}

FOnMoveSelected &UBattleMoveSelect::GetOnMoveSelected() {
    return OnMoveSelected;
}

void UBattleMoveSelect::ProcessConfirm_Implementation(int32 CurrentIndex) {
    Super::ProcessConfirm_Implementation(CurrentIndex);
    OnMoveSelected.Broadcast(CurrentBattler, GetSelectableOption<UBattleMovePanel>(CurrentIndex)->GetMove());
}

void UBattleMoveSelect::CreateMovePanel(const TScriptInterface<IBattleMove> &Move) {
    int32 OptionIndex = GetItemCount();
    auto Panel = WidgetTree->ConstructWidget(MovePanelClass, *FString::Format(TEXT("MovePanel{0}"), {OptionIndex}));
    Panel->SetMove(Move);
    SlotOption(Panel);
}