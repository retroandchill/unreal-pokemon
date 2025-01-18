// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"
#include "ApplyGameplayEffectToBattler.generated.h"

struct FActiveGameplayEffectHandle;
class IBattler;
class UGameplayEffect;
class UBattleMoveFunctionCode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectApplyComplete, TArray<FActiveGameplayEffectHandle>, Effects);

/**
 * 
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UApplyGameplayEffectToBattler : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Moves|Effects", meta = (DefaultToSelf = Move))
    static UApplyGameplayEffectToBattler* ApplyGameplayEffectToBattler(UBattleMoveFunctionCode* Move, const TScriptInterface<IBattler> &Battler,
                                                      TSubclassOf<UGameplayEffect> EffectClass, int32 Level,
                                                      int32 Stacks);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

private:
    UPROPERTY(BlueprintAssignable)
    FOnEffectApplyComplete OnComplete;
    
    UPROPERTY()
    TObjectPtr<UBattleMoveFunctionCode> Move;
    
    UPROPERTY()
    TScriptInterface<IBattler> Battler;
    
    UPROPERTY()
    TSubclassOf<UGameplayEffect> EffectClass;
    
    UPROPERTY()
    int32 Level;
    
    UPROPERTY()
    int32 Stacks;
    
};
