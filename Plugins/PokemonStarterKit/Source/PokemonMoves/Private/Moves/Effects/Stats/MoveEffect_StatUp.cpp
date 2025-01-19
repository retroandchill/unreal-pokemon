// "Unreal Pokémon" created by Retro & Chill.


#include "Moves/Effects/Stats/MoveEffect_StatUp.h"
#include "Battle/Moves/BattleMove.h"

UE5Coro::TCoroutine<bool> UMoveEffect_StatUp::MoveFailed(const TScriptInterface<IBattler> &User,
                                                         const TArray<TScriptInterface<IBattler>> &Targets, FForceLatentCoroutine ForceLatentCoroutine) {
    if (GetMove()->GetCategory() != EMoveDamageCategory::Status) {
        co_return false;
    }

    bool bFailed = false;
    for (auto &[StatID, Change] : StatsToChange) {
        bFailed |= !co_await UStatChangeHelpers::CanRaiseStat(User.GetObject(), User, StatID);
    }

    co_return bFailed;
}

UE5Coro::TCoroutine<> UMoveEffect_StatUp::ApplyAdditionalEffect(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    if (GetMove()->GetCategory() != EMoveDamageCategory::Status) {
        co_await ApplyStatChanges(User);
    }
}

UE5Coro::TCoroutine<> UMoveEffect_StatUp::ApplyGeneralEffect(const TScriptInterface<IBattler> &User,
    FForceLatentCoroutine ForceLatentCoroutine) {
    if (GetMove()->GetCategory() == EMoveDamageCategory::Status) {
        co_await ApplyStatChanges(User);
    }
}

UE5Coro::TCoroutine<> UMoveEffect_StatUp::ApplyStatChanges(const TScriptInterface<IBattler> &User, FForceLatentCoroutine) {
    for (auto &[StatID, Change] : StatsToChange) {
        co_await UStatChangeHelpers::ChangeBattlerStatStages(User, StatID, Change);
    }
}