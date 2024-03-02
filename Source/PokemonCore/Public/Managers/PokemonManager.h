// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

class ITrainer;

/**
 * Interior manager class that handles the Pokémon data at the C++ layer. Exposed to the editor through the
 * UPokemonSubsystem class.
 */
class FPokemonManager {

private:
	FPokemonManager();
	~FPokemonManager();

public:
	/**
	 * Get the singleton instance of the Pokémon Manager class
	 * @return The singleton instance of the Pokémon Manager class
	 */
	static FPokemonManager &GetInstance();

	/**
	 * Get the player trainer
	 * @return A reference to the player trainer character
	 */
	ITrainer &GetPlayer();

	/**
	 * Get the player trainer
	 * @return A reference to the player trainer character
	 */
	const ITrainer &GetPlayer() const;

private:
	/**
	 * The trainer that represents the player character
	 */
	TUniquePtr<ITrainer> Player;
};
