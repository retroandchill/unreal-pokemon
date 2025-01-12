// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleAnimation.h"
#include "UObject/Object.h"

#include "AnimationObject.generated.h"

/**
 * Implementation of IBattleAnimation that lacks a surrounding UObject
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta = (ShowWorldContextPin))
class POKEMONBATTLE_API UAnimationObject : public UObject, public IBattleAnimation {
    GENERATED_BODY()

  public:
    virtual UE5Coro::TCoroutine<> PlayAnimation() override;

  protected:
    /**
     * Play the animation playing the animation complete callback when the animation is complete
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Animation")
    void Play();

    /**
     * Signal that the animation is complete
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Battle|Animations")
    void AnimationComplete() const;

  private:
    /**
     * Called when the animation is complete
     */
    TSharedRef<TFutureState<int32>> OnBattleAnimationComplete = MakeShared<TFutureState<int32>>();
};
