// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Trainer.generated.h"

class IPokemon;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UTrainer : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface that defines the basic functionality of how a Trainer should behave.
 */
class POKEMONCORE_API ITrainer {
	GENERATED_BODY()
	
public:

	/**
	 * Get the trainer's list of party Pokémon
	 * @return The list of Pokémon in the order the trainer has them in
	 */
	virtual TArray<TScriptInterface<IPokemon>>& GetParty() = 0;

	/**
	 * Get the trainer's list of party Pokémon
	 * @return The list of Pokémon in the order the trainer has them in
	 */
	virtual const TArray<TScriptInterface<IPokemon>>& GetParty() const = 0;
};
