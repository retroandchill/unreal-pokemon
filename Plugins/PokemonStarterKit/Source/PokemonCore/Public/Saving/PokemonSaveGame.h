// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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
    TScriptInterface<ITrainer> PlayerCharacter;

    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    TScriptInterface<IBag> Bag;

    UPROPERTY(VisibleAnywhere, Category = PlayerInfo)
    TObjectPtr<UPlayerMetadata> PlayerMetadata;
    
    UPROPERTY(VisibleAnywhere, Category = Location)
    FString CurrentMap;

    UPROPERTY(VisibleAnywhere, Category = Location)
    FTransform PlayerLocation;

    UPROPERTY(VisibleAnywhere, Category = SaveMetadata)
    FDateTime SaveDate;

};
