// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/MovePanel.h"
#include "Pokemon/Moves/Move.h"

const TScriptInterface<IMove> &UMovePanel::GetMove() {
    return Move;
}

void UMovePanel::SetMove(const TScriptInterface<IMove> &MoveIn) {
    Move = MoveIn;
    OnMoveSet(MoveIn);
}

bool UMovePanel::IsMoveToLearn() const {
    return bIsMoveToLearn;
}

void UMovePanel::SetIsMoveToLearn(bool bIsBeingLearned) {
    bIsMoveToLearn = bIsBeingLearned;
}