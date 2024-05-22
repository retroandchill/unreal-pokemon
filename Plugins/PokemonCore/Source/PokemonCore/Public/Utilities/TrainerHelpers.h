// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TrainerHelpers.generated.h"

class ITrainer;

/**
 * Helper library for managing the player.
 */
UCLASS()
class POKEMONCORE_API UTrainerHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the player character.
     * @param WorldContext The world context used to retrieve the game data.
     * @return The player trainer.
     */
    UFUNCTION(BlueprintPure, Category = "Player", meta = (WorldContext = "WorldContext"))
    static const TScriptInterface<ITrainer> &GetPlayerCharacter(const UObject *WorldContext);
};
