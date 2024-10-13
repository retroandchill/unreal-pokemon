// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "PromptReplaceMove.generated.h"

struct FMoveHandle;
class IPokemon;
class UMoveForgetScreen;

/**
 * Delegate called when move learning is complete
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveLearnComplete);

/**
 * Prompt to forget a move and replace it with a new one
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UPromptReplaceMove : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    /**
     * Prompt to forget a move and replace it with a new one
     * @param WorldContextObject The object used to obtain the context about the world
     * @param Pokemon The Pokémon to learn a new move
     * @param Move The move to learn
     * @return The created node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Selection",
              meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = Move))
    static UPromptReplaceMove *PromptReplaceMove(const UObject *WorldContextObject,
                                                 const TScriptInterface<IPokemon> &Pokemon, const FMoveHandle& Move);

    void Activate() override;

  private:
    /**
     * Called when the window is closed
     * @param bMoveReplaced Was the move replaced
     */
    void OnMoveSelectionComplete(bool bMoveReplaced);

    /**
     * Called when the move was learned
     */
    UPROPERTY(BlueprintAssignable)
    FMoveLearnComplete MoveLearned;

    /**
     * Called when the move was not learned
     */
    UPROPERTY(BlueprintAssignable)
    FMoveLearnComplete MoveNotLearned;

    /**
     * The object used to obtain the context about the world
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContextObject;

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
