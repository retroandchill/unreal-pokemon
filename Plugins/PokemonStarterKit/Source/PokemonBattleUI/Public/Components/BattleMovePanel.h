// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Primatives/SelectableOption.h"
#include "BattleMovePanel.generated.h"

class UDisplayText;
class UImage;
class IBattleMove;
/**
 * The panel used to select a move from a battler
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleMovePanel : public USelectableOption {
    GENERATED_BODY()

public:
    /**
     * Set the move this panel represents
     * @param MoveIn The move to select
     */
    void SetMove(const TScriptInterface<IBattleMove>& MoveIn);

protected:
    /**
     * Get the icon for the move's type.
     * @return The move's type icon
     */
    UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Moves|Graphics")
    UObject *GetTypeIcon();

private:
    /**
     * Called when the move is set.
     */
    void OnMoveSet();

    /**
     * The move this panel holds
     */
    UPROPERTY()
    TScriptInterface<IBattleMove> Move;

    /**
     * Image icon for a move's type
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> TypeIcon;

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
    UPROPERTY(EditAnywhere, DisplayName = "Move PP Format", Category = Display)
    FText MovePPFormat = FText::FromStringView(TEXT("{0}/{1}"));
};
