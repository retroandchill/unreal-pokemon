// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
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
class POKEMONUI_API UDisplayMessage : public UBlueprintAsyncActionBase {
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
	 * @param ScreenClass The class used to display the message to the screen
	 * @param Message The message to display to the player
	 * @return The node to execute the task with
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
		Category = "Messages")
	static UDisplayMessage* DisplayMessage(const UObject* WorldContextObject,
	                                       TSubclassOf<UTextDisplayScreen> ScreenClass, FText Message);

	void Activate() override;

private:
	/**
	 * Function called to execute the on confirm pin
	 */
	UFUNCTION()
	void ExecuteOnConfirm();

	/**
	 * The object used to obtain the state of the world to display the message with 
	 */
	UPROPERTY()
	TObjectPtr<const UObject> WorldContextObject;

	/**
	 * The class used to display the message to the screen
	 */
	TSubclassOf<UTextDisplayScreen> ScreenClass;

	/**
	 * The message to display to the player
	 */
	FText Message;
};
