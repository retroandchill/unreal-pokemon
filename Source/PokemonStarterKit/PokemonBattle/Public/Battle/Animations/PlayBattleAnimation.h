// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Animations/BattleAnimation.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "PlayBattleAnimation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleAnimationComplete);

/**
 * Play a battle animation and wait for it to complete
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UPlayBattleAnimation : public UBlueprintCoroutineActionBase
{
    GENERATED_BODY()

  public:
    /**
     * Play the given battle animation
     * @param Animation The animation to play
     * @return The created animation node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Animations",
              meta = (WorldContext = WorldContext))
    static UPlayBattleAnimation *PlayBattleAnimation(const TScriptInterface<IBattleAnimation> &Animation);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    /**
     * Called when the animation is complete
     */
    UPROPERTY(BlueprintAssignable)
    FOnBattleAnimationComplete AnimationComplete;

    /**
     * The animation to play
     */
    UPROPERTY()
    TScriptInterface<IBattleAnimation> Animation;
};
