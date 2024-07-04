// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleAnimation.generated.h"

/**
 * Delegate used for the conclusion of the animation
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleAnimationComplete);

/**
 * Delegate used for the conclusion of the animation
 */
DECLARE_DYNAMIC_DELEGATE(FBattleAnimationCompleteCallback);

// This class does not need to be modified.
UINTERFACE()
class UBattleAnimation : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface used for animations played in battle
 */
class POKEMONBATTLE_API IBattleAnimation {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Play the animation playing the animation complete callback when the animation is complete
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Animation")
    void Play();

    /**
     * Bind a delegate to the animation complete callback
     * @param Delegate The delegate to bind call back to
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Animation")
    void BindDelegateToAnimationComplete(const FBattleAnimationCompleteCallback& Delegate);

    /**
     * Remove a delegate from the animation complete callback
     * @param Delegate The delegate to remove the binding of
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Animation")
    void RemoveDelegateFromAnimationComplete(const FBattleAnimationCompleteCallback& Delegate);

};