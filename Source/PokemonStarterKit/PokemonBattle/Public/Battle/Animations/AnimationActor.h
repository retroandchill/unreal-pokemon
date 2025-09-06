// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleAnimation.h"
#include "GameFramework/Actor.h"

#include "AnimationActor.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class POKEMONBATTLE_API AAnimationActor : public AActor, public IBattleAnimation
{
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
    void AnimationComplete();

  private:
    /**
     * Called when the animation is complete
     */
    TUniquePtr<TPromise<int32>> OnBattleAnimationComplete;
};