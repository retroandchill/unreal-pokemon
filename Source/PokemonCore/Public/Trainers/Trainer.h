// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

class IPokemon;

/**
 * Interface that defines the basic functionality of how a Trainer should behave.
 */
class POKEMONCORE_API ITrainer {
public:
	virtual ~ITrainer() = default;

	/**
	 * Get the trainer's list of party Pokémon
	 * @return The list of Pokémon in the order the trainer has them in
	 */
	virtual const TArray<TSharedRef<IPokemon>>& GetParty() = 0;
};
