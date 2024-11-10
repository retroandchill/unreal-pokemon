// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleMovePanel.h"
#include "Battle/Moves/BattleMove.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"

void UBattleMovePanel::SetMove(const TScriptInterface<IBattleMove> &MoveIn) {
    Move = MoveIn;
    OnMoveSet(MoveIn);
}