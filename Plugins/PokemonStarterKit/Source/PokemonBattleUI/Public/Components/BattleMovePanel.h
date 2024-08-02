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
    const TScriptInterface<IBattleMove> &GetMove() const;

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
    void SetPanelAppearance();

  private:
    /**
     * Called when the move is set.
     */
    void OnMoveSet();

    /**
     * The move this panel holds
     */
    UPROPERTY(BlueprintGetter = GetMove, Category = Content)
    TScriptInterface<IBattleMove> Move;

    /**
     * Text for a move's name
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> MoveNameText;

    /**
     * Text for the move's PP
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> MovePPText;

    /**
     * The format used to print a move's PP.
     */
    UPROPERTY(EditAnywhere, DisplayName = "Move PP Format", Category = Content)
    FText MovePPFormat = FText::FromStringView(TEXT("{0}/{1}"));
};
