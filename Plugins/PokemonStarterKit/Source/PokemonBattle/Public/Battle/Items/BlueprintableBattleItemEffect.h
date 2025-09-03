// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleItemEffect.h"

#include "BlueprintableBattleItemEffect.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class POKEMONBATTLE_API UBlueprintableBattleItemEffect : public UBattleItemEffect {
    GENERATED_BODY()

  protected:
    UE5Coro::TCoroutine<bool> ApplyGlobalEffect(TScriptInterface<IBattler> User, FForceLatentCoroutine) final;
    UE5Coro::TCoroutine<bool> ApplyEffectToTarget(TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target,
                                                  FForceLatentCoroutine) final;
    UE5Coro::TCoroutine<bool> IsTargetValid(TScriptInterface<IBattler> Battler, FForceLatentCoroutine = {}) final;

    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Items|Usable")
    void StartApplyGlobalEffect(const TScriptInterface<IBattler> &User);

    UFUNCTION(BlueprintCallable, Category = "Battle|Items|Usable")
    void CompleteApplyGlobalEffect(bool bResult);

    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Items|Usable")
    void StartApplyEffectToTarget(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target);

    UFUNCTION(BlueprintCallable, Category = "Battle|Items|Usable")
    void CompleteApplyEffectToTarget(bool bResult);

    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Items|Usable")
    void StartIsTargetValidCheck(const TScriptInterface<IBattler> &Battler);

    UFUNCTION(BlueprintCallable, Category = "Battle|Items|Usable")
    void CompleteIsTargetValidCheck(bool bResult);

  private:
    TUniquePtr<TPromise<bool>> StageCompleteState;
};
