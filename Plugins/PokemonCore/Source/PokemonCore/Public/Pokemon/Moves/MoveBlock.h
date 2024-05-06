// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "MoveBlock.generated.h"

class IPokemon;
struct FPokemonDTO;
class IMove;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UMoveBlock : public UInterface {
    GENERATED_BODY()
};

/**
 * The block of code for handling a Pokémon's moves
 */
class POKEMONCORE_API IMoveBlock {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Construct a move block from the DTO
     * @param DTO The source Pokémon DTO to initialize from
     * @return The created move block
     */
    virtual TScriptInterface<IMoveBlock> Initialize(const FPokemonDTO &DTO) = 0;

    /**
     * Get the list of moves usable by this Pokémon
     * @return The list of usable moves
     */
    UFUNCTION(BlueprintCallable, Category = "Moves")
    virtual const TArray<TScriptInterface<IMove>> &GetMoves() const = 0;
};