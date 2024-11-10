// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "BattleMovePanel.generated.h"

class UDisplayText;
class UImage;
class IBattleMove;
/**
 * The panel used to select a move from a battler
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleMovePanel : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    /**
     * Get the move for this panel
     * @return The move that this panel represents
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const TScriptInterface<IBattleMove> &GetMove() const{
        return Move;
    }

    /**
     * Set the move this panel represents
     * @param MoveIn The move to select
     */
    void SetMove(const TScriptInterface<IBattleMove> &MoveIn);

  protected:
    /**
     * Set the visual appearance of the move for this widget
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Style")
    void OnMoveSet(const TScriptInterface<IBattleMove>& NewMove);

  private:
    /**
     * The move this panel holds
     */
    UPROPERTY(BlueprintGetter = GetMove, Category = Content)
    TScriptInterface<IBattleMove> Move;
};
