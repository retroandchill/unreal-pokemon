// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "PromptSaveGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveWindowExit);

/**
 * Prompt the player to save the game and then continue based on the result.
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UPromptSaveGame : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Prompt the player to save the game and then continue based on the result.
     * @param WorldContext The screen to save the game with.
     * @return The node to process on
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Saving, meta = (WorldContext = WorldContext))
    static UPromptSaveGame *PromptToSave(const UObject *WorldContext);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro = {}) override;

  private:
    /**
     * Called when the player saves the game
     */
    UPROPERTY(BlueprintAssignable)
    FOnSaveWindowExit Saved;

    /**
     * Called when the player does not save
     */
    UPROPERTY(BlueprintAssignable)
    FOnSaveWindowExit DidNotSave;

    UPROPERTY()
    TObjectPtr<const UObject> WorldContext;
};
