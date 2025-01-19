// "Unreal Pokémon" created by Retro & Chill.

#include "Moves/Effects/Stats/MoveEffect_TargetStatDown.h"
#include "Battle/Moves/BattleMove.h"

UE5Coro::TCoroutine<bool> UMoveEffect_TargetStatDown::FailsAgainstTarget(const TScriptInterface<IBattler> &User,
                                                                         const TScriptInterface<IBattler> &Target,
                                                                         FForceLatentCoroutine ForceLatentCoroutine) {
    if (GetMove()->GetCategory() != EMoveDamageCategory::Status) {
        co_return false;
    }

    bool bFailed = false;
    for (auto &[StatID, Change] : StatsToChange) {
        bFailed |= !co_await UStatChangeHelpers::CanLowerStat(User.GetObject(), Target, StatID);
    }

    co_return bFailed;
}

UE5Coro::TCoroutine<> UMoveEffect_TargetStatDown::ApplyAdditionalEffect(const TScriptInterface<IBattler> &User,
                                                                        const TScriptInterface<IBattler> &Target) {
    if (GetMove()->GetCategory() == EMoveDamageCategory::Status) {
        co_return;
    }

    co_await ApplyStatChanges(Target);
}

UE5Coro::TCoroutine<> UMoveEffect_TargetStatDown::ApplyEffectAgainstTarget(const TScriptInterface<IBattler> &User,
                                                                           const TScriptInterface<IBattler> &Target,
                                                                           FForceLatentCoroutine ForceLatentCoroutine) {
    if (GetMove()->GetCategory() != EMoveDamageCategory::Status) {
        co_return;
    }

    co_await ApplyStatChanges(Target);
}

UE5Coro::TCoroutine<> UMoveEffect_TargetStatDown::ApplyStatChanges(const TScriptInterface<IBattler> &Target,
                                                                   FForceLatentCoroutine) {
    for (auto &[StatID, Change] : StatsToChange) {
        co_await UStatChangeHelpers::ChangeBattlerStatStages(Target, StatID, -Change);
    }
}