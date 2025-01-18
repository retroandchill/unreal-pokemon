// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Items/BlueprintableBattleItemEffect.h"

UE5Coro::TCoroutine<bool> UBlueprintableBattleItemEffect::ApplyGlobalEffect(const TScriptInterface<IBattler> &User,
    FForceLatentCoroutine) {
    if (StageCompleteState->IsComplete()) {
        StageCompleteState = MakeShared<TFutureState<bool>>();
    }

    StartApplyGlobalEffect(User);
    co_return co_await TFuture<bool>(StageCompleteState);
}

UE5Coro::TCoroutine<bool> UBlueprintableBattleItemEffect::ApplyEffectToTarget(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, FForceLatentCoroutine) {
    if (StageCompleteState->IsComplete()) {
        StageCompleteState = MakeShared<TFutureState<bool>>();
    }

    StartApplyEffectToTarget(User, Target);
    co_return co_await TFuture<bool>(StageCompleteState);
}

UE5Coro::TCoroutine<bool> UBlueprintableBattleItemEffect::IsTargetValid(const TScriptInterface<IBattler> &Battler,
    FForceLatentCoroutine) {
    if (StageCompleteState->IsComplete()) {
        StageCompleteState = MakeShared<TFutureState<bool>>();
    }

    StartIsTargetValidCheck(Battler);
    co_return co_await TFuture<bool>(StageCompleteState);
}

void UBlueprintableBattleItemEffect::StartApplyGlobalEffect_Implementation(const TScriptInterface<IBattler> &User) {
    CompleteApplyGlobalEffect(false);
}

void UBlueprintableBattleItemEffect::CompleteApplyGlobalEffect(bool bResult) {
    StageCompleteState->EmplaceResult(bResult);
}

void UBlueprintableBattleItemEffect::StartApplyEffectToTarget_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    CompleteApplyEffectToTarget(false);
}

void UBlueprintableBattleItemEffect::CompleteApplyEffectToTarget(bool bResult) {
    StageCompleteState->EmplaceResult(bResult);
}

void UBlueprintableBattleItemEffect::StartIsTargetValidCheck_Implementation(const TScriptInterface<IBattler> &Battler) {
    CompleteIsTargetValidCheck(false);
}

void UBlueprintableBattleItemEffect::CompleteIsTargetValidCheck(bool bResult) {
    StageCompleteState->EmplaceResult(bResult);
}
