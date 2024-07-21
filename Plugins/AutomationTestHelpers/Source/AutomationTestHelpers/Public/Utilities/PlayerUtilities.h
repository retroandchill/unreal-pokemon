// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RAII.h"

#include "PlayerUtilities.generated.h"

/**
 * Blueprint function library for utilities that revolve around player operations.
 */
UCLASS()
class AUTOMATIONTESTHELPERS_API UPlayerUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Create the test player, wrapping it in a container that will remove it upon the conclusion of the test.
     * @param World The world to create the player in
     * @param PawnClass
     * @return The created player and pawn in the world
     */
    static std::pair<FPlayerPtr, APawn *> CreateTestPlayer(UWorld &World, const TSubclassOf<APawn> &PawnClass = APawn::StaticClass());
};
