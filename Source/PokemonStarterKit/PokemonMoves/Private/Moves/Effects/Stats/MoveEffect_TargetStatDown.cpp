// "Unreal Pokémon" created by Retro & Chill.

#include "Moves/Effects/Stats/MoveEffect_TargetStatDown.h"
#include "Battle/Moves/BattleMove.h"

UE5Coro::TCoroutine<bool> UMoveEffect_TargetStatDown::FailsAgainstTarget(TScriptInterface<IBattler> User,
                                                                         TScriptInterface<IBattler> Target,
                                                                         FForceLatentCoroutine ForceLatentCoroutine)
{
    if (GetMove()->GetCategory() != EMoveDamageCategory::Status)
    {
        co_return false;
    }

    bool bFailed = false;
    for (const auto &[StatID, Change] : StatsToChange)
    {
        bFailed |= !co_await UStatChangeHelpers::CanLowerStat(User.GetObject(), Target, StatID);
    }

    co_return bFailed;
}

UE5Coro::TCoroutine<> UMoveEffect_TargetStatDown::ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                                        TScriptInterface<IBattler> Target)
{
    if (GetMove()->GetCategory() == EMoveDamageCategory::Status)
    {
        co_return;
    }

    co_await ApplyStatChanges(Target);
}

UE5Coro::TCoroutine<> UMoveEffect_TargetStatDown::ApplyEffectAgainstTarget(TScriptInterface<IBattler> User,
                                                                           TScriptInterface<IBattler> Target,
                                                                           FForceLatentCoroutine ForceLatentCoroutine)
{
    if (GetMove()->GetCategory() != EMoveDamageCategory::Status)
    {
        co_return;
    }

    co_await ApplyStatChanges(Target);
}

UE5Coro::TCoroutine<> UMoveEffect_TargetStatDown::ApplyStatChanges(const TScriptInterface<IBattler> &Target,
                                                                   FForceLatentCoroutine)
{
    for (const auto &[StatID, Change] : StatsToChange)
    {
        co_await UStatChangeHelpers::ChangeBattlerStatStages(Target, StatID, -Change);
    }
}