// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UE5Coro.h"

namespace Pokemon::UI
{

    /**
     * Callback delegate for updating the progress bar
     */
    DECLARE_DELEGATE_OneParam(FSetNewPercent, float);

    /**
     * Called when an animation is complete
     */
    DECLARE_MULTICAST_DELEGATE(FOnAnimationComplete);

    /**
     * Struct that contains all the information needed to update a progress bar
     */
    struct POKEMONUI_API FBarAnimationData
    {

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
         * Should the animation wrap around when the percentage gets to 1?
         */
        bool bWrapAround = false;

        /**
         * Construct a new animation
         * @param StartingPercentage The starting percentage for the animation
         * @param EndingPercentage The ending percentage of the animation
         * @param AnimationDuration The duration of the animation
         * @param bWrapAround Should the animation wrap around when the percentage gets to 1?
         */
        FBarAnimationData(float StartingPercentage, float EndingPercentage, float AnimationDuration,
                          bool bWrapAround = false);
    };

    /**
     * Play an animation from start to finish with the following parameters
     * @param Context
     * @param StartPercent The starting percentage of the progress bar
     * @param EndPercent The ending percentage of the progress bar
     * @param Duration How long it should take to get there
     * @param OnUpdate
     * @param bShouldWrap
     * @param OnWrapAround
     */
    POKEMONUI_API UE5Coro::TCoroutine<> ProgressBarAnimation(UE5Coro::TLatentContext<const UObject> Context,
                                                             float StartPercent, float EndPercent, float Duration,
                                                             FSetNewPercent OnUpdate, bool bShouldWrap = false,
                                                             FSimpleDelegate OnWrapAround = FSimpleDelegate());

} // namespace Pokemon::UI