// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "GainExpOnFaint.generated.h"

class UPokemonBattleScreen;
class IBattler;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExpGainComplete);

/**
 * Calculate and gain exp for the provided battlers who fainted.
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLEUI_API UGainExpOnFaint : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Animations",
              meta = (WorldContext = WorldContextObject))
    static UGainExpOnFaint *GainExpOnFaint(const UObject *WorldContextObject,
                                           const TArray<TScriptInterface<IBattler>> &Battlers);

    void Activate() override;

  private:
    void OnExpGainComplete();

    UPROPERTY()
    TObjectPtr<const UObject> WorldContextObject;

    UPROPERTY()
    TArray<TScriptInterface<IBattler>> Battlers;

    UPROPERTY(BlueprintAssignable)
    FExpGainComplete OnComplete;

    FDelegateHandle CompleteHandle;
};
