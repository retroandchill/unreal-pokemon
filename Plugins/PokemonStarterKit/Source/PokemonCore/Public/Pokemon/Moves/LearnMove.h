// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "LearnMove.generated.h"

class IPokemon;

/**
 * Delegate called after move learning has been completed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAfterMoveLearn);

/**
 * Teach a move to a specific Pokémon
 */
UCLASS(meta = (HideThen))
class POKEMONCORE_API ULearnMove : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    /**
     * Teach a move to a specific Pokémon
     * @param Pokemon The Pokémon to learn a new move
     * @param Move The move to learn
     * @return The created async action
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Pokémon|Moves")
    static ULearnMove *LearnMove(const TScriptInterface<IPokemon> &Pokemon, FName Move);

    void Activate() override;

  private:
    /**
     * Exit the node on either a rejection or an acceptance
     * @param bMoveLearned Was the move learned or not
     */
    UFUNCTION()
    void ExecuteMoveLearnedOrRejected(bool bMoveLearned);

    /**
     * Called after the move is learned
     */
    UPROPERTY(BlueprintAssignable)
    FAfterMoveLearn MoveLearned;

    /**
     * Called after the move is rejected
     */
    UPROPERTY(BlueprintAssignable)
    FAfterMoveLearn MoveRejected;

    /**
     * The Pokémon to learn a new move
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The move to learn
     */
    UPROPERTY()
    FName Move;
};
