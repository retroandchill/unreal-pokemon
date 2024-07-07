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
    TScriptInterface<IMoveBlock> Initialize(const TScriptInterface<IPokemon>& Pokemon, const FPokemonDTO &DTO) override;

    const TArray<TScriptInterface<IMove>> &GetMoves() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    bool HasOpenMoveSlot() const;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    void PlaceMoveInOpenSlot(FName Move) override;
    
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    void OverwriteMoveSlot(FName Move, int32 SlotIndex) override;
    
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Pokémon|Moves")
    virtual TArray<FName> GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const;
    
    void LearnMove(FName Move, const FMoveLearnEnd& AfterMoveLearned) override;

  private:
    UPROPERTY()
    TScriptInterface<IPokemon> Owner;
    
    UPROPERTY()
    TArray<TScriptInterface<IMove>> Moves;

    /**
     * The full move learning memory of the Pokémon presented in the order the moves were learned.
     */
    TSet<FName> MoveMemory;
};
