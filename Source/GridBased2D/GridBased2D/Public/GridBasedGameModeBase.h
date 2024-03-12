// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GridBasedGameModeBase.generated.h"

/**
 * Game Mode class used to define the information about all of the characters in the game
 */
UCLASS(Blueprintable)
class GRIDBASED2D_API AGridBasedGameModeBase : public AGameMode {
	GENERATED_BODY()

public:
	/**
	 * Initialize the GameMode to use the C++ defined classes
	 */
	AGridBasedGameModeBase();
};
