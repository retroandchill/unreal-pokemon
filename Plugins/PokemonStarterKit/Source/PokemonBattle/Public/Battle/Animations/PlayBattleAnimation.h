// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Battle/Animations/BattleAnimation.h"
#include "PlayBattleAnimation.generated.h"

/**
 * Play a battle animation and wait for it to complete
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UPlayBattleAnimation : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Play the given battle animation
     * @param Animation The animation to play
     * @return The created animation node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Animations")
    static UPlayBattleAnimation* PlayBattleAnimation(const TScriptInterface<IBattleAnimation>& Animation);

    void Activate() override;
    
private:
    /**
     * Called when the animation is complete
     */
    UFUNCTION()
    void OnAnimationComplete();

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

    /**
     * The delegate to call back to
     */
    TUniquePtr<FBattleAnimationCompleteCallback> AnimationDelegate;
};
