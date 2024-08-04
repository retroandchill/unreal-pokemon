// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameDataSavingLibrary.h"
#include "GameFramework/SaveGame.h"
#include "Trainers/TrainerDTO.h"

#include "PokemonSaveGame.generated.h"

class UPlayerMetadata;
class IBag;
class ITrainer;
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
    FObjectData Bag;

    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    FObjectData PlayerMetadata;

    UPROPERTY(VisibleAnywhere, Category = Location)
    FString CurrentMap;

    UPROPERTY(VisibleAnywhere, Category = Location)
    FTransform PlayerLocation;

    UPROPERTY(VisibleAnywhere, Category = SaveMetadata)
    FDateTime SaveDate;
};
