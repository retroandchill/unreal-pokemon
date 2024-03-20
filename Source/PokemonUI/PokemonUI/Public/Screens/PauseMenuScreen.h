// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Screens/Screen.h"
#include "PauseMenuScreen.generated.h"

class UCommand;
class UCommandWindow;
/**
 * Basic Pokémon pause menu screen
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UPauseMenuScreen : public UScreen {
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;

private:
	/**
	 * Process the selected command
	 * @param CurrentIndex The index of the command
	 * @param SelectedCommand The command in question
	 */
	UFUNCTION()
	void ProcessCommand(int32 CurrentIndex, UCommand* SelectedCommand);

	/**
	 * The command window that is displayed to the player
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommandWindow> CommandWindow;

	/**
	 * The sound that plays when the menu is opened.
	 */
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> OpenSound;
};
