// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleAnimation.h"
#include "UObject/Object.h"

#include "AnimationObject.generated.h"

/**
 * Implementation of IBattleAnimation that lacks a surrounding UObject
 */
UCLASS(BlueprintType, Blueprintable, meta = (ShowWorldContextPin))
class POKEMONBATTLE_API UAnimationObject : public UObject, public IBattleAnimation {
    GENERATED_BODY()

  public:
    void BindDelegateToAnimationComplete_Implementation(const FBattleAnimationCompleteCallback &Delegate) override;
    void RemoveDelegateFromAnimationComplete_Implementation(const FBattleAnimationCompleteCallback &Delegate) override;

  protected:
    /**
     * Signal that the animation is complete
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Battle|Animations")
    void AnimationComplete() const;

  private:
    /**
     * Called when the animation is complete
     */
    UPROPERTY()
    FOnBattleAnimationComplete OnBattleAnimationComplete;
};
