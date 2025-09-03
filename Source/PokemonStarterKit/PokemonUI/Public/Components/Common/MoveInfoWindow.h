// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MoveInfoWindow.generated.h"

struct FMoveData;
class UDisplayText;
class UImage;
class IMove;
enum class EMoveDamageCategory : uint8;

/**
 * Widget that displays information about a particular move.
 */
UCLASS(Abstract)
class POKEMONUI_API UMoveInfoWindow : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Retrieves the current move data for this widget.
     * This is a pure function that is intended for internal blueprint use only.
     *
     * @return A constant reference to the current move interface.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<IMove> &GetMove() const {
        return Move;
    }

    /**
     * Sets the new move for this widget and updates the display accordingly.
     *
     * @param NewMove The new move to set for this widget.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetMove(const TScriptInterface<IMove> &NewMove);

  protected:
    /**
     * Refresh the information to the given move.
     *
     * @param NewMove The move to display
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void OnMoveSet(const TScriptInterface<IMove> &NewMove);

    /**
     * Event triggered when a move is cleared from the widget.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void OnMoveCleared();

  private:
    /**
     * Interface for accessing and modifying the move data in this widget.
     * The getter function retrieves the current move, while the setter allows modification of the move.
     */
    UPROPERTY(BlueprintGetter = GetMove, BlueprintSetter = SetMove, Category = Move)
    TScriptInterface<IMove> Move;
};
