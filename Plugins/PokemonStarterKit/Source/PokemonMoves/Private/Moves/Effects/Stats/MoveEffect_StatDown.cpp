// "Unreal Pokémon" created by Retro & Chill.


#include "Moves/Effects/Stats/MoveEffect_StatDown.h"

UE5Coro::TCoroutine<> UMoveEffect_StatDown::ApplyEffectWhenDealingDamage(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, FForceLatentCoroutine ForceLatentCoroutine) {
    for (auto &[StatID, Change] : StatsToChange) {
        co_await UStatChangeHelpers::ChangeBattlerStatStages(User, StatID, -Change);
    }
}