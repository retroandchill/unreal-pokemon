﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/BattleMoveSelect.h"
#include "Algo/ForEach.h"
#include "Battle/Battlers/Battler.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMovePanel.h"
#include <functional>

void UBattleMoveSelect::SetBattler(const TScriptInterface<IBattler> &NewBattler) {
    CurrentBattler = NewBattler;
    Algo::ForEach(MovePanels, &UWidget::RemoveFromParent);
    MovePanels.Reset();
    Algo::ForEach(NewBattler->GetMoves(), std::bind_front(&UBattleMoveSelect::CreateMovePanel, this));
}

FOnMoveSelected &UBattleMoveSelect::GetOnMoveSelected() {
    return OnMoveSelected;
}

int32 UBattleMoveSelect::GetItemCount_Implementation() const {
    return MovePanels.Num();
}

void UBattleMoveSelect::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    if (MovePanels.IsValidIndex(OldIndex)) {
        MovePanels[OldIndex]->OnUnselected();
    }

    if (MovePanels.IsValidIndex(NewIndex)) {
        MovePanels[NewIndex]->OnSelected();
    }
}

void UBattleMoveSelect::ProcessConfirm_Implementation(int32 CurrentIndex) {
    Super::ProcessConfirm_Implementation(CurrentIndex);
    check(MovePanels.IsValidIndex(CurrentIndex))
    OnMoveSelected.Broadcast(CurrentBattler, MovePanels[CurrentIndex]->GetMove());
}

void UBattleMoveSelect::CreateMovePanel(const TScriptInterface<IBattleMove> &Move) {
    int32 OptionIndex = MovePanels.Num();
    auto Panel = WidgetTree->ConstructWidget(MovePanelClass, *FString::Format(TEXT("MovePanel{0}"), {OptionIndex}));
    Panel->SetMove(Move);
    SlotWidget(Panel);
    Panel->SetOptionIndex(OptionIndex);
    Panel->GetOnOptionClicked().AddDynamic(this, &UBattleMoveSelect::ProcessClickedButton);
    Panel->GetOnOptionHovered().AddDynamic(this, &UBattleMoveSelect::ProcessHoveredButton);
    MovePanels.Emplace(Panel);
}