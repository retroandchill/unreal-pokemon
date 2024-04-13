// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBasedGameModeBase.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "FadeScreenIn.generated.h"

/**
 * Async Node for fading the screen in
 */
UCLASS()
class GRIDBASED2D_API UFadeScreenIn : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    /**
     * Fade the screen in
     * @param WorldContext The object used to obtain the world information
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Screen Transitions",
              meta = (WorldContext = WorldContext))
    static UFadeScreenIn *FadeScreenIn(const UObject *WorldContext);

    void Activate() override;

  private:
    /**
     * Called when the screen transition completes
     */
    UFUNCTION()
    void TransitionFinished();

    /**
     * Called when the transition is finished
     */
    UPROPERTY(BlueprintAssignable)
    FOnScreenTransitionFinished OnScreenTransitionFinished;

    /**
     * The object used to obtain the world information
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContext;
};
