﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Move.generated.h"

struct FMoveData;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UMove : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface for a basic Pokémon move
 */
class POKEMONCORE_API IMove {
	GENERATED_BODY()
	
public:
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

	/**
	 * Check to see if two moves are the same
	 * @param Other The other move
	 * @return Are these two moves the same?
	 */
	virtual bool Equals(const TScriptInterface<IMove>& Other) const = 0;
};
