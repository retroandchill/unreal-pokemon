// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "DisplayMessage.generated.h"

class UTextDisplayScreen;

/**
 * Delegate for handling the display output pin
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisplayMessageOutputPin);

/**
 * Displays a message and waits for the player to press the advance button.
 */
UCLASS(meta = (HideThen, HasDedicatedAsyncNode))
class POKEMONUI_API UDisplayMessage : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Called when the player presses the button to advance the dialogue
     */
    UPROPERTY(BlueprintAssignable)
    FDisplayMessageOutputPin OnConfirm;

    /**
     * Display a message to the player and call the following after the message is done
     * @param WorldContextObject The object used to obtain the state of the world to display the message with
     * @param Message The message to display to the player
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Messages")
    static UDisplayMessage *DisplayMessage(const UObject *WorldContextObject, FText Message);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro = {}) override;

  private:
    /**
     * The message to display to the player
     */
    UPROPERTY()
    FText Message;
};
