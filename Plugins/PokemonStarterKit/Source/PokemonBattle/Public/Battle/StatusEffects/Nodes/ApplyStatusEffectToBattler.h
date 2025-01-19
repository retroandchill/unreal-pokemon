// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Battle/Status.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "ApplyStatusEffectToBattler.generated.h"

class IBattler;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectApplied, FActiveGameplayEffectHandle, EffectHandle);

/**
 *
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UApplyStatusEffectToBattler : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Status Effects")
    static UApplyStatusEffectToBattler *ApplyStatusEffectToBattler(const TScriptInterface<IBattler> &Target,
                                                                   FStatusHandle StatusEffectID);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    UPROPERTY(BlueprintAssignable)
    FOnStatusEffectApplied OnComplete;

    UPROPERTY()
    TScriptInterface<IBattler> Target;

    UPROPERTY()
    FStatusHandle StatusEffectID;
};
