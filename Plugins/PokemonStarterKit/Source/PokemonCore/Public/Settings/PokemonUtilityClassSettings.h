// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonUtilityClassSettings.generated.h"

/**
 * Settings related to the utility class interfaces used by the game for utility functions that may involve displaying
 * text to the player/asking for player input.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon Utility Functions")
class POKEMONCORE_API UPokemonUtilityClassSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The class for the Pokémon utilities
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Utility Classes",
              meta = (MustImplement = "PokemonUtilities"))
    FSoftClassPath PokemonUtilitiesClass;

    /**
     * The blueprint class that contains the item utilities information
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Utility Classes",
              meta = (MustImplement = "ItemUtilities"))
    FSoftClassPath ItemUtilitiesClass;
};
