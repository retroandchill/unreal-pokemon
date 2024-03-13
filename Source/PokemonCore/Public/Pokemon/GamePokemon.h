// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon.h"
#include "Memory/RowPointer.h"
#include "Stats/StatBlock.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"

struct FPokemonDTO;
class IMove;

/**
 * Basic Pokémon class that holds all of the information for a complete Pokémon
 */
class POKEMONCORE_API FGamePokemon : public IPokemon {
public:
	/**
	 * Construct a Pokémon from the DTO
	 * @param DTO The source Pokémon DTO to initialize from
	 */
	explicit FGamePokemon(const FPokemonDTO& DTO);

	FGamePokemon(FGamePokemon&& Other) noexcept = default;

	FGamePokemon& operator=(FGamePokemon&& Other) noexcept = default;

	FText GetName() const override;
	const FSpeciesData& GetSpecies() const override;
	EPokemonGender GetGender() const override;
	int32 GetCurrentHP() const override;
	int32 GetMaxHP() const override;
	bool IsFainted() const override;
	const IStatBlock& GetStatBlock() const override;

private:
	/**
	 * The ID of the species of Pokémon this is
	 */
	TRowPointer<FSpeciesData> Species;

	/**
	 * The internal personality value of the Pokémon. Determines the default values of various aspects of the
	 * Pokémon if the values are not already set.
	 */
	uint32 PersonalityValue;

	/**
	 * The nickname assigned to the Pokémon. Uses the species name if empty.
	 */
	TOptional<FText> Nickname;

	/**
	 * The hardcoded gender of the Pokémon. Calculates using the personality value is unset.
	 */
	TOptional<EPokemonGender> Gender;

	/**
	 * The hardcoded shiny status of the Pokémon. Calculates using the personality value is unset.
	 */
	TOptional<bool> Shiny;

	/**
	 * The current amount of HP this Pokémon has
	 */
	int32 CurrentHP;

	/**
	 * The handler for calculating stats
	 */
	TUniquePtr<IStatBlock> StatBlock;

	/**
	 * The moves this Pokémon knows
	 */
	TArray<TSharedRef<IMove>> Moves;
};
