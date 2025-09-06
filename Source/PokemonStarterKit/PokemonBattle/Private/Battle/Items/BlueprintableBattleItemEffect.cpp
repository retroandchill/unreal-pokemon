// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/BlueprintableBattleItemEffect.h"

UE5Coro::TCoroutine<bool> UBlueprintableBattleItemEffect::ApplyGlobalEffect(TScriptInterface<IBattler> User,
                                                                            FForceLatentCoroutine)
{

    StageCompleteState = MakeUnique<TPromise<bool>>();
    StartApplyGlobalEffect(User);
    co_return co_await StageCompleteState->GetFuture();
}

UE5Coro::TCoroutine<bool> UBlueprintableBattleItemEffect::ApplyEffectToTarget(TScriptInterface<IBattler> User,
                                                                              TScriptInterface<IBattler> Target,
                                                                              FForceLatentCoroutine)
{
    StageCompleteState = MakeUnique<TPromise<bool>>();
    StartApplyEffectToTarget(User, Target);
    co_return co_await StageCompleteState->GetFuture();
}

UE5Coro::TCoroutine<bool> UBlueprintableBattleItemEffect::IsTargetValid(TScriptInterface<IBattler> Battler,
                                                                        FForceLatentCoroutine)
{
    StageCompleteState = MakeUnique<TPromise<bool>>();
    StartIsTargetValidCheck(Battler);
    co_return co_await StageCompleteState->GetFuture();
}

void UBlueprintableBattleItemEffect::StartApplyGlobalEffect_Implementation(const TScriptInterface<IBattler> &User)
{
    CompleteApplyGlobalEffect(false);
}

void UBlueprintableBattleItemEffect::CompleteApplyGlobalEffect(bool bResult)
{
    StageCompleteState->EmplaceValue(bResult);
}

void UBlueprintableBattleItemEffect::StartApplyEffectToTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                                             const TScriptInterface<IBattler> &Target)
{
    CompleteApplyEffectToTarget(false);
}

void UBlueprintableBattleItemEffect::CompleteApplyEffectToTarget(bool bResult)
{
    StageCompleteState->EmplaceValue(bResult);
}

void UBlueprintableBattleItemEffect::StartIsTargetValidCheck_Implementation(const TScriptInterface<IBattler> &Battler)
{
    CompleteIsTargetValidCheck(false);
}

void UBlueprintableBattleItemEffect::CompleteIsTargetValidCheck(bool bResult)
{
    StageCompleteState->EmplaceValue(bResult);
}
