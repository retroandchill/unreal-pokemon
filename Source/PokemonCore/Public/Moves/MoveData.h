// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

struct FMoveData;
/**
 * Interface for a basic Pokémon move
 */
class POKEMONCORE_API IMove {
public:
	virtual ~IMove() = default;

	/**
	 * Get the data for the underlying move that this move represents
	 * @return The data for this move
	 */
	virtual const FMoveData &GetMoveData() const = 0;

	/**
	 * Get the current PP for the move
	 * @return The current PP for the move
	 */
	virtual int32 GetCurrentPP() const = 0;

	/**
	 * Get the total PP for this move
	 * @return The total PP for the move
	 */
	virtual int32 GetTotalPP() const = 0;
};
