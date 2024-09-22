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
    TScriptInterface<IMoveBlock> Initialize(const TScriptInterface<IPokemon> &Pokemon, const FPokemonDTO &DTO) override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    const TArray<TScriptInterface<IMove>> &GetMoves() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    bool HasOpenMoveSlot() const;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    void PlaceMoveInOpenSlot(FMoveHandle Move) override;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    void OverwriteMoveSlot(FMoveHandle Move, int32 SlotIndex) override;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Pokémon|Moves")
    virtual TArray<FMoveHandle> GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const;

    void LearnMove(FMoveHandle Move, FOnMoveLearnEnd::FDelegate &&AfterMoveLearned) override;
    TScriptInterface<IMove> CreateNewMove(const FMoveDTO &MoveID) override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    const TSet<FName> &GetMoveMemory() const override;

  private:
    UPROPERTY(SaveGame)
    TScriptInterface<IPokemon> Owner;

    UPROPERTY(SaveGame)
    TArray<TScriptInterface<IMove>> Moves;

    /**
     * The full move learning memory of the Pokémon presented in the order the moves were learned.
     */
    UPROPERTY(SaveGame)
    TSet<FName> MoveMemory;
};
