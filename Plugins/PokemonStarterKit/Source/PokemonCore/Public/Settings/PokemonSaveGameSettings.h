// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Saving/PokemonSaveGame.h"

#include "PokemonSaveGameSettings.generated.h"

class UPokemonSaveGame;
/**
 * The settings for saving the game.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon Save Files")
class POKEMONCORE_API UPokemonSaveGameSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    /**
     * The name of the primary save slot
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Saving & Loading")
    FString PrimarySaveSlotName = TEXT("PokemonSaveGame");

    /**
     * The index of the primary save slot
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Saving & Loading")
    int32 PrimarySaveIndex = 0;

    /**
     * The type of the save game class
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Saving & Loading",
        meta = (MetaClass = "PokemonSaveGame"))
    FSoftClassPath SaveGameClass = UPokemonSaveGame::StaticClass();

};
