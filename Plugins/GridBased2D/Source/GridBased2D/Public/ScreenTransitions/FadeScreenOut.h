﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBasedGameModeBase.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "FadeScreenOut.generated.h"

/**
 * Async Node for fading the screen out
 */
UCLASS()
class GRIDBASED2D_API UFadeScreenOut : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Fade the screen in
     * @param WorldContextObject The object used to obtain the world information
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Screen Transitions",
              meta = (WorldContext = WorldContextObject))
    static UFadeScreenOut *FadeScreenOut(const UObject *WorldContextObject);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    /**
     * Called when the transition is finished
     */
    UPROPERTY(BlueprintAssignable)
    FOnScreenTransitionFinished OnScreenTransitionFinished;
};
