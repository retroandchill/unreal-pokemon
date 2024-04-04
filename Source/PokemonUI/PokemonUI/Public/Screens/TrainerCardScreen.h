// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "TrainerCardScreen.generated.h"

class ITrainer;
class UImage;
class UDisplayText;

/**
 * The screen used to bring up the trainer card.
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UTrainerCardScreen : public UScreen {
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	/**
	 * Set the graphic used for the trainer's sprite
	 */
	void SetTrainerSprite();

	/**
	 * Set the textual information for the player
	 */
	void SetTrainerInfo();
	
	/**
	 * The trainer we're displaying information for
	 */
	UPROPERTY()
	TScriptInterface<ITrainer> Trainer;
	
	/**
	 * The image used for the player's sprite
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> TrainerImage;

	/**
	 * The text used to display the player's name
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDisplayText> TrainerName;
	
};
