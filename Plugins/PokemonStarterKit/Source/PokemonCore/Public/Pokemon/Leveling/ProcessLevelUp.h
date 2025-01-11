// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Pokemon/Stats/StatBlock.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"
#include "UE5Coro.h"

#include "ProcessLevelUp.generated.h"

class IPokemon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAfterLevelUp);

/**
 *
 */
UCLASS(meta = (HideThen))
class POKEMONCORE_API UProcessLevelUp : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Pokémon|Stats")
    static UProcessLevelUp *ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon,
                                           const FLevelUpStatChanges &StatChanges, bool bShowMessages);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro) override;

  private:
    /**
     * Called after level up messages are shown
     */
    UPROPERTY(BlueprintAssignable)
    FAfterLevelUp AfterLevelUp;

    /**
     * The Pokémon to gain exp
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The amount of Exp. to gain
     */
    UPROPERTY()
    FLevelUpStatChanges StatChanges;

    /**
     * Should level up messages be show? (Hiding messages does not allow Pokémon to learn moves on level up)
     */
    UPROPERTY()
    bool bShowMessages;
};
