// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMoveFunctionCode.h"

#include "BlueprintableMoveFunctionCode.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class POKEMONBATTLE_API UBlueprintableMoveFunctionCode : public UBattleMoveFunctionCode {
    GENERATED_BODY()

  public:
    UE5Coro::TCoroutine<bool> MoveFailed(TScriptInterface<IBattler> User,
                                         const TArray<TScriptInterface<IBattler>> &Targets,
                                         FForceLatentCoroutine = {}) final;

    UE5Coro::TCoroutine<bool> FailsAgainstTarget(TScriptInterface<IBattler> User,
                                                 TScriptInterface<IBattler> Target,
                                                 FForceLatentCoroutine = {}) final;

    UE5Coro::TCoroutine<> ApplyEffectWhenDealingDamage(TScriptInterface<IBattler> User,
                                                       TScriptInterface<IBattler> Target,
                                                       FForceLatentCoroutine = {}) final;

    UE5Coro::TCoroutine<> ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                TScriptInterface<IBattler> Target) final;

    UE5Coro::TCoroutine<> ApplyEffectAgainstTarget(TScriptInterface<IBattler> User,
                                                   TScriptInterface<IBattler> Target,
                                                   FForceLatentCoroutine = {}) final;

    UE5Coro::TCoroutine<> ApplyGeneralEffect(TScriptInterface<IBattler> User, FForceLatentCoroutine = {}) final;

  protected:
    UFUNCTION(BlueprintNativeEvent, Category = "Move|Checks")
    void StartMoveFailedCheck(const TScriptInterface<IBattler> &User,
                              const TArray<TScriptInterface<IBattler>> &Targets);

    UFUNCTION(BlueprintCallable, Category = "Move|Checks")
    void CompleteMoveFailedCheck(bool bFailed);

    UFUNCTION(BlueprintNativeEvent, Category = "Move|Checks")
    void StartFailsAgainstTargetCheck(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target);

    UFUNCTION(BlueprintCallable, Category = "Move|Checks")
    void CompleteFailsAgainstTargetCheck(bool bFailed);

    UFUNCTION(BlueprintNativeEvent, Category = "Move|Effects")
    void StartApplyEffectWhenDealingDamage(const TScriptInterface<IBattler> &User,
                                           const TScriptInterface<IBattler> &Target);

    UFUNCTION(BlueprintCallable, Category = "Move|Effects")
    void CompleteApplyEffectWhenDealingDamage();

    UFUNCTION(BlueprintNativeEvent, Category = "Move|Effects")
    void StartApplyAdditionalEffect(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target);

    UFUNCTION(BlueprintCallable, Category = "Move|Effects")
    void CompleteApplyAdditionalEffect();

    UFUNCTION(BlueprintNativeEvent, Category = "Move|Effects")
    void StartApplyEffectAgainstTarget(const TScriptInterface<IBattler> &User,
                                       const TScriptInterface<IBattler> &Target);

    UFUNCTION(BlueprintCallable, Category = "Move|Effects")
    void CompleteApplyEffectAgainstTarget();

    UFUNCTION(BlueprintNativeEvent, Category = "Move|Effects")
    void StartApplyGeneralEffect(const TScriptInterface<IBattler> &User);

    UFUNCTION(BlueprintCallable, Category = "Move|Effects")
    void CompleteApplyGeneralEffect();

  private:
    TSharedRef<TFutureState<bool>> CoroutineFutureState = MakeShared<TFutureState<bool>>();
};
