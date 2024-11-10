// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "MovePanel.generated.h"

class UDisplayText;
class UImage;
class IMove;
/**
 * Abstract panel that displays information about a Pokémon's moves.
 */
UCLASS(Abstract)
class POKEMONUI_API UMovePanel : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    /**
     * Get the move for the panel.
     * @return The actual move that this panel is displaying.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Moves)
    const TScriptInterface<IMove> &GetMove();

    /**
     * Set the move for the panel.
     * @param MoveIn The actual move that this panel is displaying.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Moves)
    void SetMove(const TScriptInterface<IMove> &MoveIn);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Moves)
    bool IsMoveToLearn() const;

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Moves)
    void SetIsMoveToLearn(bool bIsBeingLearned);

  protected:
    /**
     * Called when the move is set.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Moves)
    void OnMoveSet(const TScriptInterface<IMove> &NewMove);

private:
    /**
     * The actual move that this panel is displaying.
     */
    UPROPERTY(BlueprintGetter = GetMove, BlueprintSetter = SetMove, Category = Moves)
    TScriptInterface<IMove> Move;

    UPROPERTY(BlueprintGetter = IsMoveToLearn, BlueprintSetter = SetIsMoveToLearn, Category = Moves)
    bool bIsMoveToLearn;
};
