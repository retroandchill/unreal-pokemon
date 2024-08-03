// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BattleMove.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/Battler.h"

static int32 GetTargetIndex(const TScriptInterface<IBattler> &Battler) {
    auto &Battlers = Battler->GetOwningSide()->GetBattlers();
    auto OwnIndex = Battlers.Find(Battler);
    check(Battlers.IsValidIndex(OwnIndex))
    return OwnIndex;
}

FTargetWithIndex::FTargetWithIndex(const TScriptInterface<IBattler> &Battler)
    : Target(Battler.GetObject()), BattlerIndex(GetTargetIndex(Battler))  {
}