// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleMovePanel.h"
#include "Battle/Moves/BattleMove.h"

void UBattleMovePanel::SetMove(const TScriptInterface<IBattleMove> &MoveIn) {
    Move = MoveIn;
    OnMoveSet(MoveIn);
}