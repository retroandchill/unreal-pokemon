// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Status.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "CanStatusEffectBeInflicted.generated.h"

class IBattler;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatusCanBeInflicted, bool, ReturnValue);

/**
 *
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UCanStatusEffectBeInflicted : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Status Effects")
    static UCanStatusEffectBeInflicted *CanStatusEffectBeInflicted(const TScriptInterface<IBattler> &Target,
                                                                   FStatusHandle StatusEffectID,
                                                                   FText AlreadyAppliedFormat,
                                                                   FText HasOtherStatusFormat);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    UPROPERTY(BlueprintAssignable)
    FStatusCanBeInflicted OnComplete;

    UPROPERTY()
    TScriptInterface<IBattler> Target;

    UPROPERTY()
    FStatusHandle StatusEffectID;

    UPROPERTY()
    FText AlreadyAppliedFormat;

    UPROPERTY()
    FText HasOtherStatusFormat;
};
