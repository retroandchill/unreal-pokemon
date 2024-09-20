// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleMovePanel.h"
#include "Battle/Moves/BattleMove.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"

const TScriptInterface<IBattleMove> &UBattleMovePanel::GetMove() const {
    return Move;
}

void UBattleMovePanel::SetMove(const TScriptInterface<IBattleMove> &MoveIn) {
    Move = MoveIn;
    OnMoveSet();
}

void UBattleMovePanel::OnMoveSet() {
    using enum ESlateVisibility;
    check(Move != nullptr)
    MoveNameText->SetText(Move->GetDisplayName());
    MovePPText->SetText(FText::Format(MovePPFormat, {Move->GetCurrentPP(), Move->GetMaxPP()}));
    SetPanelAppearance();
}