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
	UPROPERTY(EditAnywhere, Category = Data, meta = (EditCondition=bOverride_PersonalityValue))
	uint32 PersonalityValue;

	/**
	 * The override state of the personality value
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	bool bOverride_PersonalityValue;

	/**
	 * The nickname assigned to the Pokémon. Uses the species name if empty.
	 */
	UPROPERTY(EditAnywhere, Category = Data, meta = (EditCondition=bOverride_Nickname))
	FText Nickname;

	/**
	 * The override state of the nickname
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	bool bOverride_Nickname;

	/**
	 * The hardcoded gender of the Pokémon. Calculates using the personality value is unset.
	 */
	UPROPERTY(EditAnywhere, Category = Data, meta = (EditCondition=bOverride_Gender))
	EPokemonGender Gender;

	/**
	 * The override state of the gender
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	bool bOverride_Gender;

	/**
	 * The hardcoded shiny status of the Pokémon. Calculates using the personality value is unset.
	 */
	UPROPERTY(EditAnywhere, Category = Data, meta = (EditCondition=bOverride_Shiny))
	bool bShiny;

	/**
	 * The override state of the shiny status
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	bool bOverride_Shiny;

	/**
	 * The current amount of HP this Pokémon has
	 */
	UPROPERTY(EditAnywhere, Category = Data, meta = (EditCondition=bOverride_CurrentHP))
	int32 CurrentHP;

	/**
	 * The override state of the nickname
	 */
	UPROPERTY(EditAnywhere, Category = Data)
	bool bOverride_CurrentHP;

	/**
	 * The current amount of HP this Pokémon has
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	FStatBlockDTO StatBlock;
};
