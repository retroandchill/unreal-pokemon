// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBasedGameModeBase.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FadeScreenOut.generated.h"

/**
 * Async Node for fading the screen out
 */
UCLASS()
class GRIDBASED2D_API UFadeScreenOut : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    /**
     * Called when the transition is finished
     */
    UPROPERTY(BlueprintAssignable)
    FOnScreenTransitionFinished OnScreenTransitionFinished;

    /**
     * Fade the screen in
     * @param WorldContext The object used to obtain the world information
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Screen Transitions",
              meta = (WorldContext = WorldContext))
    static UFadeScreenOut *FadeScreenOut(const UObject *WorldContext);

    void Activate() override;

  private:
    /**
     * Called when the screen transition completes
     */
    UFUNCTION()
    void TransitionFinished();

    /**
     * The object used to obtain the world information
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContext;
};
