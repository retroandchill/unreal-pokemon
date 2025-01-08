// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "DisplayMessageWithChoices.generated.h"

class UTextDisplayScreen;
/**
 * Delegate for handling the display output pin
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDisplayMessageWithChoicesOutputPin, int32, Index, FName, Choice);

/**
 * Command for displaying a message with choices associated with it
 */
UCLASS(meta = (HideThen, HasDedicatedAsyncNode))
class POKEMONUI_API UDisplayMessageWithChoices : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Called when the player makes a choice
     */
    UPROPERTY(BlueprintAssignable)
    FDisplayMessageWithChoicesOutputPin OnChoiceSelected;

    /**
     * Display a message to the player and call the following after the a choice as been selected
     * @param WorldContextObject The object used to obtain the state of the world to display the message with
     * @param ScreenClass The class used to display the message to the screen
     * @param Message The message to display to the player
     * @param Choices The choices available to the player
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Display Message w/ Choices",
              meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Messages")
    static UDisplayMessageWithChoices *DisplayMessageWithChoices(const UObject *WorldContextObject, FText Message,
                                                                 const TArray<FText> &Choices);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro = {}) override;

  private:
    /**
     * The message to display to the player
     */
    UPROPERTY()
    FText Message;

    /**
     * The choices to display
     */
    UPROPERTY()
    TArray<FText> Choices;
};
