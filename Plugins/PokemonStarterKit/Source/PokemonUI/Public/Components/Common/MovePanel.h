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
     * The actual move that this panel is displaying.
     */
    UPROPERTY(BlueprintGetter = GetMove, BlueprintSetter = SetMove, Category = Moves)
    TScriptInterface<IMove> Move;

    UPROPERTY(BlueprintGetter = IsMoveToLearn, BlueprintSetter = SetIsMoveToLearn, Category = Moves)
    bool bIsMoveToLearn;

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

    /**
     * The PP displayed for a blank move.
     */
    UPROPERTY(EditAnywhere, Category = Display)
    FText UnknownMoveLabel = FText::FromStringView(TEXT("-----"));

    /**
     * The PP displayed for a blank move
     */
    UPROPERTY(EditAnywhere, Category = Display)
    FText UnknownMovePP = FText::FromStringView(TEXT("--"));
};
