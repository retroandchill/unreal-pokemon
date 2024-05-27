// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "MoveBlock.h"
#include "UObject/Object.h"

#include "DefaultMoveBlock.generated.h"

/**
 * The default implementation for a Pokémon's move block.
 */
UCLASS()
class POKEMONCORE_API UDefaultMoveBlock : public UObject, public IMoveBlock {
    GENERATED_BODY()

  public:
    /**
     * Constant for the maximum number of moves for this type
     */
    static constexpr int32 MaxMoves = 4;
    
    TScriptInterface<IMoveBlock> Initialize(const FPokemonDTO &DTO) override;

    TConstArrayView<TScriptInterface<IMove>> GetMoves() const override;
    
    UFUNCTION(BlueprintPure, Category = Moves)
    int32 GetMaxMoves() const override;

private:
    UPROPERTY()
    TArray<TScriptInterface<IMove>> Moves;

    /**
     * The full move learning memory of the Pokémon presented in the order the moves were learned.
     */
    TSet<FName> MoveMemory;
};
