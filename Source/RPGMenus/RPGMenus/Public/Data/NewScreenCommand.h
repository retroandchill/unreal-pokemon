// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "NewScreenCommand.generated.h"

class UScreen;

/**
 * Command class to handle transfering to a new screen
 */
UCLASS()
class RPGMENUS_API UNewScreenCommand : public UCommand {
	GENERATED_BODY()

public:
	void ExecuteCommand_Implementation(ARPGPlayerController* Controller) override;

private:
	/**
	 * The next screen to transfer the player to
	 */
	UPROPERTY(EditAnywhere, Category = Effect, meta = (DisplayAfter = Text))
	TSubclassOf<UScreen> NextScreen;
};
