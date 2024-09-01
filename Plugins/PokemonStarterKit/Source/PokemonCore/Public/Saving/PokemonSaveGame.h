// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Player/BagDTO.h"
#include "Trainers/TrainerDTO.h"

#include "PokemonSaveGame.generated.h"

/**
 * The structure of a save file for the game.
 */
UCLASS()
class POKEMONCORE_API UPokemonSaveGame : public USaveGame {
    GENERATED_BODY()

  public:
    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    FTrainerDTO PlayerCharacter;

    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    FBagDTO Bag;

    UPROPERTY(VisibleAnywhere, Category = Location)
    FString CurrentMap;

    UPROPERTY(VisibleAnywhere, Category = Location)
    FTransform PlayerLocation;

    UPROPERTY(VisibleAnywhere, Category = Reset)
    FString ResetMap;

    UPROPERTY(VisibleAnywhere, Category = Reset)
    FTransform ResetLocation;

    UPROPERTY(VisibleAnywhere, Category = SaveMetadata)
    FDateTime SaveDate;

    UPROPERTY(VisibleAnywhere, Category = SaveMetadata)
    FDateTime StartDate;

    UPROPERTY(VisibleAnywhere, Category = SaveMetadata)
    float TotalPlaytime;

    UPROPERTY(VisibleAnywhere, Category = SaveMetadata)
    int32 RepelSteps;
};
