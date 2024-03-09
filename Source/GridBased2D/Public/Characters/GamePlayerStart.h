// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GamePlayerStart.generated.h"

/**
 * Player start class with a capsule that matches the size of the grid.
 */
UCLASS(Blueprintable)
class GRIDBASED2D_API AGamePlayerStart : public APlayerStart {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 * @param ObjectInitializer The initialization parameters of the object
	 */
	explicit AGamePlayerStart(const FObjectInitializer& ObjectInitializer);
	
};
