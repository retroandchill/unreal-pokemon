// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/BagDTO.h"
#include "Saving/Serialization/Serializable.h"
#include "Storage/StorageSystemDTO.h"
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
} // namespace Pokemon::Saving

/**
 * The structure of a save file for the game.
 */
UCLASS()
class POKEMONCORE_API UPokemonSaveGame : public UObject, public ISerializable {
    GENERATED_BODY()

  public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo)
    FTrainerDTO PlayerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo)
    FBagDTO Bag;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo)
    FStorageSystemDTO StorageSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Location)
    FString CurrentMap;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Location)
    FTransform PlayerLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reset)
    FString ResetMap;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reset)
    FTransform ResetLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveMetadata)
    FDateTime SaveDate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveMetadata)
    FDateTime StartDate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveMetadata)
    float TotalPlaytime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveMetadata)
    int32 RepelSteps;
};
