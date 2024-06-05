// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TrainerHelpers.generated.h"

class IBag;
class ITrainer;

/**
 * Helper library for managing the player.
 */
UCLASS(Blueprintable)
class POKEMONCORE_API UTrainerHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the player character.
     * @param WorldContextObject The world context used to retrieve the game data.
     * @return The player trainer.
     */
    UFUNCTION(BlueprintPure, Category = "Player", meta = (WorldContext = "WorldContextObject"))
    static const TScriptInterface<ITrainer> &GetPlayerCharacter(const UObject *WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Player", meta = (WorldContext = "WorldContextObject"))
    static const TScriptInterface<IBag>& GetBag(const UObject *WorldContextObject);
};
