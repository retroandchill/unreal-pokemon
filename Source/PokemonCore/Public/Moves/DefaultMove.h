// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Moves/Move.h"
#include "Memory/RowPointer.h"
#include "Moves/MoveData.h"

/**
 * The default implementation for a move
 */
class POKEMONCORE_API FDefaultMove : public IMove {
public:
	explicit FDefaultMove(FName MoveID);
	
	const FMoveData& GetMoveData() const override;
	int32 GetCurrentPP() const override;
	int32 GetTotalPP() const override;

private:
	/**
	 * A reference to the move data struct
	 */
	TRowPointer<FMoveData> MoveData;

	/**
	 * The current PP for this move
	 */
	int32 CurrentPP; 
};
