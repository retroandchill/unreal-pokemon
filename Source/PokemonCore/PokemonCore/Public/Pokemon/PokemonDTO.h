// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Breeding/PokemonGender.h"
#include "Stats/StatBlockDTO.h"
#include "PokemonDTO.generated.h"

/**
 * Data transfer object for a Pokémon. This is what gets serialized for both the builder and for any communication
 * over the network.
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FPokemonDTO {
	GENERATED_BODY()

	/**
	 * The ID of the species this Pokémon represents
	 */
	UPROPERTY(EditAnywhere, Category = "Data")
	FName Species;

	/**
	 * The internal personality value of the Pokémon. Determines the default values of various aspects of the
	 * Pokémon if the values are not already set.
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	TOptional<uint32> PersonalityValue;

	/**
	 * The nickname assigned to the Pokémon. Uses the species name if empty.
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	TOptional<FText> Nickname;

	/**
	 * The hardcoded gender of the Pokémon. Calculates using the personality value is unset.
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	TOptional<EPokemonGender> Gender;

	/**
	 * The hardcoded shiny status of the Pokémon. Calculates using the personality value is unset.
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	TOptional<bool> bShiny;

	/**
	 * The current amount of HP this Pokémon has
	 */
	UPROPERTY(EditAnywhere, Category = Data, meta = (EditCondition=bOverride_CurrentHP))
	TOptional<int32> CurrentHP;

	/**
	 * The current amount of HP this Pokémon has
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	FStatBlockDTO StatBlock;
};
