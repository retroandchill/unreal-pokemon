// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Animations/ProgressBarAnimation.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "UpdateHPBar.generated.h"

class UScreen;
class IPokemon;
class UProgressBar;

/**
 * Update a Pokémon's HP bar to its new desired value.
 */
UCLASS(DisplayName = "Update HP Bar", meta = (HideThen))
class POKEMONUI_API UUpdateHPBar : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationComplete);

    /**
     * Update a Pokémon's HP bar to its new desired value.
     * @param Screen The screen that is currently being shown
     * @param Pokemon The Pokémon to use the HP value of
     * @param MaxDuration The maximum amount of time this animation should take
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Animations")
    static UUpdateHPBar *UpdateHPBar(UScreen *Screen, const TScriptInterface<IPokemon> &Pokemon,
                                     float MaxDuration = 1.f);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    /**
     * Called when the HP bar is done animating
     */
    UPROPERTY(BlueprintAssignable)
    FOnAnimationComplete OnAnimationComplete;

    UPROPERTY()
    TObjectPtr<UScreen> Screen;

    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    float MaxDuration;
};
