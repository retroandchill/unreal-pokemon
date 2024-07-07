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
     * @param Pokemon
     * @param DTO The source Pokémon DTO to initialize from
     * @return The created move block
     */
    virtual TScriptInterface<IMoveBlock> Initialize(const TScriptInterface<IPokemon>& Pokemon, const FPokemonDTO &DTO) = 0;

    /**
     * Get the list of moves usable by this Pokémon
     * @return The list of usable moves
     */
    virtual const TArray<TScriptInterface<IMove>> &GetMoves() const = 0;

    /**
     * Get the level up moves that are learned betwen level ups
     * @param InitialLevel The level that leveling started at
     * @param CurrentLevel The current level of the Pokémon
     * @return The moves that will be learned
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Pokémon|Moves")
    virtual TArray<FName> GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const = 0;
};