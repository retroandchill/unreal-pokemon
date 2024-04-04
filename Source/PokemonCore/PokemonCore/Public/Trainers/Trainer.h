// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Trainer.generated.h"

struct FTrainerType;
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
	 * Get the type of the trainer this is
	 * @return The type of trainer this is
	 */
	virtual const FTrainerType &GetTrainerType() const = 0;

	/**
	 * Get the name of the trainer
	 * @return The trainer's name
	 */
	virtual FText GetName() const = 0;

	/**
	 * Get the amount of money this trainer will pay out
	 * @return The amount of money this trainer pays out
	 */
	virtual uint32 GetPayout() const = 0;

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

	/**
	 * Get the trainer's ID number
	 * @return The trainer's ID number
	 */
	virtual int32 GetIdNumber() = 0;

	/**
	 * Get the trainer's secret ID number
	 * @return The trainer's secret ID number
	 */
	virtual int32 GetSecretId() = 0;
};
