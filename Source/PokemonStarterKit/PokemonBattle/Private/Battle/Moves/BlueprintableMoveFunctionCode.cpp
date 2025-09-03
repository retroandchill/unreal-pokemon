// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BlueprintableMoveFunctionCode.h"

UE5Coro::TCoroutine<bool> UBlueprintableMoveFunctionCode::MoveFailed(TScriptInterface<IBattler> User,
                                                                     const TArray<TScriptInterface<IBattler>> &Targets,
                                                                     FForceLatentCoroutine) {
    CoroutineFutureState = MakeUnique<TPromise<bool>>();
    StartMoveFailedCheck(User, Targets);
    co_return co_await CoroutineFutureState->GetFuture();
}

UE5Coro::TCoroutine<bool> UBlueprintableMoveFunctionCode::FailsAgainstTarget(TScriptInterface<IBattler> User,
                                                                             TScriptInterface<IBattler> Target,
                                                                             FForceLatentCoroutine) {
    CoroutineFutureState = MakeUnique<TPromise<bool>>();
    StartFailsAgainstTargetCheck(User, Target);
    co_return co_await CoroutineFutureState->GetFuture();
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyEffectWhenDealingDamage(TScriptInterface<IBattler> User,
                                                                                   TScriptInterface<IBattler> Target,
                                                                                   FForceLatentCoroutine) {
    CoroutineFutureState = MakeUnique<TPromise<bool>>();
    StartApplyEffectWhenDealingDamage(User, Target);
    co_await CoroutineFutureState->GetFuture();
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                                            TScriptInterface<IBattler> Target) {
    CoroutineFutureState = MakeUnique<TPromise<bool>>();
    StartApplyAdditionalEffect(User, Target);
    co_await CoroutineFutureState->GetFuture();
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyEffectAgainstTarget(TScriptInterface<IBattler> User,
                                                                               TScriptInterface<IBattler> Target,
                                                                               FForceLatentCoroutine) {
    CoroutineFutureState = MakeUnique<TPromise<bool>>();
    StartApplyEffectAgainstTarget(User, Target);
    co_await CoroutineFutureState->GetFuture();
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyGeneralEffect(TScriptInterface<IBattler> User,
                                                                         FForceLatentCoroutine) {
    CoroutineFutureState = MakeUnique<TPromise<bool>>();
    StartApplyGeneralEffect(User);
    co_await CoroutineFutureState->GetFuture();
}

void UBlueprintableMoveFunctionCode::StartMoveFailedCheck_Implementation(
    const TScriptInterface<IBattler> &User, const TArray<TScriptInterface<IBattler>> &Targets) {
    CompleteMoveFailedCheck(false);
}

void UBlueprintableMoveFunctionCode::CompleteMoveFailedCheck(bool bFailed) {
    CoroutineFutureState->EmplaceValue(bFailed);
}

void UBlueprintableMoveFunctionCode::StartFailsAgainstTargetCheck_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteFailsAgainstTargetCheck(false);
}

void UBlueprintableMoveFunctionCode::CompleteFailsAgainstTargetCheck(bool bFailed) {
    CoroutineFutureState->EmplaceValue(bFailed);
}

void UBlueprintableMoveFunctionCode::StartApplyEffectWhenDealingDamage_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteApplyEffectWhenDealingDamage();
}

void UBlueprintableMoveFunctionCode::CompleteApplyEffectWhenDealingDamage() {
    CoroutineFutureState->EmplaceValue(true);
}

void UBlueprintableMoveFunctionCode::StartApplyAdditionalEffect_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteApplyAdditionalEffect();
}

void UBlueprintableMoveFunctionCode::CompleteApplyAdditionalEffect() {
    CoroutineFutureState->EmplaceValue(true);
}

void UBlueprintableMoveFunctionCode::StartApplyEffectAgainstTarget_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteApplyEffectAgainstTarget();
}

void UBlueprintableMoveFunctionCode::CompleteApplyEffectAgainstTarget() {
    CoroutineFutureState->EmplaceValue(true);
}

void UBlueprintableMoveFunctionCode::StartApplyGeneralEffect_Implementation(const TScriptInterface<IBattler> &User) {
    CompleteApplyGeneralEffect();
}

void UBlueprintableMoveFunctionCode::CompleteApplyGeneralEffect() {
    CoroutineFutureState->EmplaceValue(true);
}