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
    TScriptInterface<IMoveBlock> Initialize(const FPokemonDTO &DTO) override;

    const TArray<TScriptInterface<IMove>> &GetMoves() const override;

  private:
    UPROPERTY()
    TArray<TScriptInterface<IMove>> Moves;

    /**
     * The full move learning memory of the Pokémon presented in the order the moves were learned.
     */
    TSet<FName> MoveMemory;
};
