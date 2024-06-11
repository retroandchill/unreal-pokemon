// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "MoveBlock.generated.h"

class IPokemon;
struct FPokemonDTO;
class IMove;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType, meta = (Injectable))
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
    virtual TConstArrayView<TScriptInterface<IMove>> GetMoves() const = 0;

    /**
     * Get the move on the Pokémon that is used essentially as a last resort option when there are no remaining options
     * to choose from.
     * @return The move of last resort to use in battle
     */
    virtual const TScriptInterface<IMove>& GetMoveOfLastResort() const = 0;
};