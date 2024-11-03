// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/BagDTO.h"
#include "Storage/StorageSystemDTO.h"
#include "Saving/Serialization/Serializable.h"
#include "Trainers/TrainerDTO.h"

#include "PokemonSaveGame.generated.h"

namespace Pokemon::Saving {
    /**
     * The native gameplay tag for the save data slot for this plugin's information.
     */
    POKEMONCORE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(PokemonCoreSaveData);
    
    /**
     * Native gameplay tag for signalling after loading the game to change maps
     */
    POKEMONCORE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(ChangeMapOnLoad);
}

/**
 * The structure of a save file for the game.
 */
UCLASS()
class POKEMONCORE_API UPokemonSaveGame : public USaveGame, public ISerializable {
    GENERATED_BODY()

  public:
    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    FTrainerDTO PlayerCharacter;

    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    FBagDTO Bag;

    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    FStorageSystemDTO StorageSystem;

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
