// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

struct FTrainerType;
class IPokemon;

/**
 * Interface that defines the basic functionality of how a Trainer should behave.
 */
class POKEMONCORE_API ITrainer {
public:
	virtual ~ITrainer() = default;

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
	virtual TArray<TSharedRef<IPokemon>>& GetParty() = 0;

	/**
	 * Get the trainer's list of party Pokémon
	 * @return The list of Pokémon in the order the trainer has them in
	 */
	virtual const TArray<TSharedRef<IPokemon>>& GetParty() const = 0;

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
