// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Status.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"
#include "RemoveStatusEffectFromBattler.generated.h"

class IBattler;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatusEffectRemoved, bool, ReturnValue);

/**
 * 
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API URemoveStatusEffectFromBattler : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Status Effects")
    static URemoveStatusEffectFromBattler* RemoveStatusEffectFromBattler(const TScriptInterface<IBattler>& Target, FStatusHandle StatusEffectID);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;
    
private:
    UPROPERTY(BlueprintAssignable)
    FStatusEffectRemoved OnComplete;
    
    UPROPERTY()
    TScriptInterface<IBattler> Target;

    UPROPERTY()
    FStatusHandle StatusEffectID;

};
