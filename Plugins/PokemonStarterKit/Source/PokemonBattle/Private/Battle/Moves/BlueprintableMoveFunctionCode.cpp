﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BlueprintableMoveFunctionCode.h"

UE5Coro::TCoroutine<bool> UBlueprintableMoveFunctionCode::MoveFailed(TScriptInterface<IBattler> User,
                                                                     const TArray<TScriptInterface<IBattler>> &Targets,
                                                                     FForceLatentCoroutine) {
    if (CoroutineFutureState->IsComplete()) {
        CoroutineFutureState = MakeShared<TFutureState<bool>>();
    }
    StartMoveFailedCheck(User, Targets);
    co_return co_await TFuture<bool>(CoroutineFutureState);
}

UE5Coro::TCoroutine<bool> UBlueprintableMoveFunctionCode::FailsAgainstTarget(TScriptInterface<IBattler> User,
                                                                             TScriptInterface<IBattler> Target,
                                                                             FForceLatentCoroutine) {
    if (CoroutineFutureState->IsComplete()) {
        CoroutineFutureState = MakeShared<TFutureState<bool>>();
    }

    StartFailsAgainstTargetCheck(User, Target);
    co_return co_await TFuture<bool>(CoroutineFutureState);
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyEffectWhenDealingDamage(
    TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target, FForceLatentCoroutine) {
    if (CoroutineFutureState->IsComplete()) {
        CoroutineFutureState = MakeShared<TFutureState<bool>>();
    }

    StartApplyEffectWhenDealingDamage(User, Target);
    co_await TFuture<bool>(CoroutineFutureState);
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                                            TScriptInterface<IBattler> Target) {
    if (CoroutineFutureState->IsComplete()) {
        CoroutineFutureState = MakeShared<TFutureState<bool>>();
    }

    StartApplyAdditionalEffect(User, Target);
    co_await TFuture<bool>(CoroutineFutureState);
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyEffectAgainstTarget(TScriptInterface<IBattler> User,
                                                                               TScriptInterface<IBattler> Target,
                                                                               FForceLatentCoroutine) {
    if (CoroutineFutureState->IsComplete()) {
        CoroutineFutureState = MakeShared<TFutureState<bool>>();
    }

    StartApplyEffectAgainstTarget(User, Target);
    co_await TFuture<bool>(CoroutineFutureState);
}

UE5Coro::TCoroutine<> UBlueprintableMoveFunctionCode::ApplyGeneralEffect(TScriptInterface<IBattler> User,
                                                                         FForceLatentCoroutine) {
    if (CoroutineFutureState->IsComplete()) {
        CoroutineFutureState = MakeShared<TFutureState<bool>>();
    }

    StartApplyGeneralEffect(User);
    co_await TFuture<bool>(CoroutineFutureState);
}

void UBlueprintableMoveFunctionCode::StartMoveFailedCheck_Implementation(
    const TScriptInterface<IBattler> &User, const TArray<TScriptInterface<IBattler>> &Targets) {
    CompleteMoveFailedCheck(false);
}

void UBlueprintableMoveFunctionCode::CompleteMoveFailedCheck(bool bFailed) {
    CoroutineFutureState->EmplaceResult(bFailed);
}

void UBlueprintableMoveFunctionCode::StartFailsAgainstTargetCheck_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteFailsAgainstTargetCheck(false);
}

void UBlueprintableMoveFunctionCode::CompleteFailsAgainstTargetCheck(bool bFailed) {
    CoroutineFutureState->EmplaceResult(bFailed);
}

void UBlueprintableMoveFunctionCode::StartApplyEffectWhenDealingDamage_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteApplyEffectWhenDealingDamage();
}

void UBlueprintableMoveFunctionCode::CompleteApplyEffectWhenDealingDamage() {
    CoroutineFutureState->EmplaceResult(true);
}

void UBlueprintableMoveFunctionCode::StartApplyAdditionalEffect_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteApplyAdditionalEffect();
}

void UBlueprintableMoveFunctionCode::CompleteApplyAdditionalEffect() {
    CoroutineFutureState->EmplaceResult(true);
}

void UBlueprintableMoveFunctionCode::StartApplyEffectAgainstTarget_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    CompleteApplyEffectAgainstTarget();
}

void UBlueprintableMoveFunctionCode::CompleteApplyEffectAgainstTarget() {
    CoroutineFutureState->EmplaceResult(true);
}

void UBlueprintableMoveFunctionCode::StartApplyGeneralEffect_Implementation(const TScriptInterface<IBattler> &User) {
    CompleteApplyGeneralEffect();
}

void UBlueprintableMoveFunctionCode::CompleteApplyGeneralEffect() {
    CoroutineFutureState->EmplaceResult(true);
}