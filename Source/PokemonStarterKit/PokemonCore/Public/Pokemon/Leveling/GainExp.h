// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "GainExp.generated.h"

class IPokemon;

/**
 * Delegate called after exp gain and level up processing ended
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAfterExpGain);

/**
 * Gain the specified amount of experience for a Pokémon
 */
UCLASS(meta = (HideThen))
class POKEMONCORE_API UGainExp : public UBlueprintCoroutineActionBase
{
    GENERATED_BODY()

  public:
    /**
     * Gain the specified amount of experience for a Pokémon
     * @param Pokemon The Pokémon to gain exp
     * @param ExpAmount The amount of Exp. to gain
     * @param bShowMessages Should level up messages be show? (Hiding messages does not allow Pokémon to learn moves on
     * level up)
     * @return The created node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Pokémon|Stats")
    static UGainExp *GainExp(const TScriptInterface<IPokemon> &Pokemon, int32 ExpAmount, bool bShowMessages);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    /**
     * Called after level up messages are shown
     */
    UPROPERTY(BlueprintAssignable)
    FAfterExpGain AfterExpGain;

    /**
     * The Pokémon to gain exp
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The amount of Exp. to gain
     */
    UPROPERTY()
    int32 ExpAmount;

    /**
     * Should level up messages be show? (Hiding messages does not allow Pokémon to learn moves on level up)
     */
    UPROPERTY()
    bool bShowMessages;
};
