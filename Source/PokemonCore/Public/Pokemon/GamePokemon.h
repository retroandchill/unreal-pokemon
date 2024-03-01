// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon.h"
#include "Stats/StatBlock.h"

/**
 * Basic Pokémon class that holds all of the information for a complete Pokémon
 */
class POKEMONCORE_API FGamePokemon : public IPokemon {
public:
	/**
	 * Create a Pokémon with the following species and level information
	 * @param Species The species of Pokémon to create
	 * @param Level The level of the Pokémon in question
	 */
	explicit FGamePokemon(FName Species, int32 Level = 5);
	
	const FSpeciesData& GetSpecies() const override;
	const IStatBlock& GetStatBlock() const override;

private:
	/**
	 * The ID of the species of Pokémon this is
	 */
	FName Species;

	TUniquePtr<IStatBlock> StatBlock;
};
