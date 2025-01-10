// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Lookup/InjectableDependency.h"
#include "Moves/MoveData.h"
#include "Pokemon/PokemonDTO.h"
#include "RetroLib/Ranges/Views/Generator.h"
#include "UObject/Interface.h"
#include "UE5Coro.h"

#include "MoveBlock.generated.h"

class FMoveLearnEnd;
class IPokemon;
struct FPokemonDTO;
class IMove;

/**
 * The delegate called when the move learn procedure ends
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveLearnEnd, bool);

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
     * @param Pokemon
     * @param DTO The source Pokémon DTO to initialize from
     * @return The created move block
     */
    virtual TScriptInterface<IMoveBlock> Initialize(const TScriptInterface<IPokemon> &Pokemon,
                                                    const FPokemonDTO &DTO) = 0;

    /**
     * Get the list of moves usable by this Pokémon
     * @return The list of usable moves
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual const TArray<TScriptInterface<IMove>> &GetMoves() const = 0;

    /**
     * Does this Pokémon have an open move slot to learn a move in
     * @return Does this Pokémon have an open move slot
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual bool HasOpenMoveSlot() const = 0;

    /**
     * Place the newly learned move into an open move slot
     * @param Move The move to be learned
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual void PlaceMoveInOpenSlot(FMoveHandle Move) = 0;

    /**
     * Overwrite an existing move
     * @param Move The move to be learned
     * @param SlotIndex The index of the move to replace
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual void OverwriteMoveSlot(FMoveHandle Move, int32 SlotIndex) = 0;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual const TSet<FName> &GetMoveMemory() const = 0;

    /**
     * Get the level up moves that are learned between level ups
     * @param InitialLevel The level that leveling started at
     * @param CurrentLevel The current level of the Pokémon
     * @return The moves that will be learned
     */
    virtual Retro::TGenerator<FMoveHandle> GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const = 0;

    /**
     * Teach a move to this Pokémon
     * @param Move The move to learn
     * @param Coro This is called after the move learning prompt is done
     */
    virtual FVoidCoroutine LearnMove(FMoveHandle Move, FForceLatentCoroutine Coro = {}) = 0;

    /**
     * Create a new move interface object. This is typically a temporary used for the move learn screen)
     * @param MoveID The ID of the move to learn
     * @return The created move object
     */
    virtual TScriptInterface<IMove> CreateNewMove(const FMoveDTO &MoveID) = 0;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, IMoveBlock);