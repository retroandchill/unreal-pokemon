// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Moves/Move.h"
#include "DefaultMove.generated.h"

/**
 * The default implementation for a move
 */
UCLASS()
class POKEMONCORE_API UDefaultMove : public UObject, public IMove {
	GENERATED_BODY()
	
public:
	TScriptInterface<IMove> Initialize(FName MoveID);
	
	const FMoveData& GetMoveData() const override;
	int32 GetCurrentPP() const override;
	int32 GetTotalPP() const override;
	bool Equals(const TScriptInterface<IMove>& Other) const override;

	/**
	 * Check to see if two moves are the same
	 * @param Other The other move
	 * @return Are these two moves the same?
	 */
	bool operator==(const UDefaultMove& Other) const;

private:
	/**
	 * A reference to the move data struct
	 */
	UPROPERTY(SaveGame)
	FName ID;

	/**
	 * The current PP for this move
	 */
	UPROPERTY(SaveGame)
	int32 CurrentPP; 
};
