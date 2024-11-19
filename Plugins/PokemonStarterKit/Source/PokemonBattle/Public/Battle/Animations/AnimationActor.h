// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleAnimation.h"
#include "GameFramework/Actor.h"
#include "AnimationActor.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class POKEMONBATTLE_API AAnimationActor : public AActor, public IBattleAnimation {
    GENERATED_BODY()

public:
    void BindDelegateToAnimationComplete_Implementation(const FBattleAnimationCompleteCallback &Delegate) override;
    void RemoveDelegateFromAnimationComplete_Implementation(const FBattleAnimationCompleteCallback &Delegate) override;

protected:
    /**
     * Signal that the animation is complete
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Battle|Animations")
    void AnimationComplete();

private:
    /**
     * Called when the animation is complete
     */
    UPROPERTY()
    FOnBattleAnimationComplete OnBattleAnimationComplete;

};