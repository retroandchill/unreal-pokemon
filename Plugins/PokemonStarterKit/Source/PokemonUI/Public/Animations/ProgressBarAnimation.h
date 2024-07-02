// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace Pokemon::UI {

/**
 * Callback delegate for updating the progress bar
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FSetNewPercent, float)

/**
 * Called when an animation is complete
 */
DECLARE_MULTICAST_DELEGATE(FOnAnimationComplete)

/**
 * Struct that contains all the information needed to update a progress bar
 */
struct POKEMONUI_API FBarAnimationData {

    /**
     * The starting percentage for the animation
     */
    float StartingPercentage;

    /**
     * The ending percentage of the animation
     */
    float EndPercentage;

    /**
     * The duration of the animation
     */
    float AnimationDuration;

    /**
     * The current state of the timer
     */
    float CurrentTime = 0.f;

    /**
     * Construct a new animation
     * @param StartingPercentage The starting percentage for the animation
     * @param EndingPercentage The ending percentage of the animation
     * @param AnimationDuration The duration of the animation
     */
    FBarAnimationData(float StartingPercentage, float EndingPercentage, float AnimationDuration);
};

/**
 * Animation component for draining a progress bar
 */
class POKEMONUI_API FProgressBarAnimation : public FTickableGameObject {
public:
    /**
     * Play an animation from start to finish with the following parameters
     * @param StartPercent The starting percentage of the progress bar
     * @param EndPercent The ending percentage of the progress bar
     * @param Duration How long it should take to get there
     */
    void PlayAnimation(float StartPercent, float EndPercent, float Duration);

    /**
     * Bind an action to the update callback for this component
     * @param Binding The binding for the update action
     */
    void BindActionToPercentDelegate(FSetNewPercent::FDelegate&& Binding);

    /**
     * Bind an action to the complete callback for this component
     * @param Binding The binding for the complete action
     */
    void BindActionToCompleteDelegate(FOnAnimationComplete::FDelegate&& Binding);
    
    void Tick(float DeltaTime) override;
    TStatId GetStatId() const override;

private:
    /**
     * Delegate for updating the percent of the progress bar
     */
    FSetNewPercent SetNewPercent;

    /**
     * Delegate for signalling that the animation is complete
     */
    FOnAnimationComplete OnAnimationComplete;

    /**
     * The animation data for this bar. Only animates when set.
     */
    TOptional<FBarAnimationData> AnimationData;
};

}
