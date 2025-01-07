// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "SaveGameOnScreen.generated.h"

class USaveScreen;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenSaveComplete);

/**
 * Attempt to save the game.
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API USaveGameOnScreen : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Attempt to save the game.
     * @param Screen The screen to save the game with.
     * @return The node to process on
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Saving, meta = (DefaultToSelf = Screen))
    static USaveGameOnScreen *SaveGame(USaveScreen *Screen);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro = {}) override;

  private:
    /**
     * Called when the save is completed
     */
    UPROPERTY(BlueprintAssignable)
    FOnScreenSaveComplete SaveSuccess;

    UPROPERTY(BlueprintAssignable)
    FOnScreenSaveComplete SaveFailed;

    /**
     * The screen to save the game with.
     */
    UPROPERTY()
    TObjectPtr<USaveScreen> Screen;
};
