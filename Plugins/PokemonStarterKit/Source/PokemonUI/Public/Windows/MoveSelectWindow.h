// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"

#include "MoveSelectWindow.generated.h"

class IMove;
class IPokemon;
class UMovePanel;

/**
 * Delegate for when the cursor moves to another move
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveSelectionChanged, const TScriptInterface<IMove> &, Move);

/**
 * Widget that displays the Pokémon's moves in the UI.
 */
UCLASS(Abstract)
class POKEMONUI_API UMoveSelectWindow : public USelectableWidget {
    GENERATED_BODY()

  public:
    /**
     * Display the moves to the player.
     * @param Pokemon The Pokémon to display the moves for.
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    void DisplayMoves(const TScriptInterface<IPokemon> &Pokemon);

  protected:
    /**
     * Slot a created panel into this widget.
     * @param Panel The panel that was slotted
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotPanel(UMovePanel *Panel);

    int32 GetItemCount_Implementation() const override;
    void OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) override;

    /**
     * Set the position of the cursor based on the (valid) index
     * @param Widget The widget that acts as the cursor for the window
     * @param NewIndex The index of the cursor
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SetCursorPosition(UWidget *Widget, int32 NewIndex);

  private:
    /**
     * Create a new move panel
     * @param Move The move to create a panel from
     * @return The created panel
     */
    UMovePanel *CreateMovePanel(const TScriptInterface<IMove> &Move);

    /**
     * The class used for the move panels.
     */
    UPROPERTY(EditAnywhere, Category = Display)
    TSubclassOf<UMovePanel> PanelClass;

    /**
     * The created move panels.
     */
    UPROPERTY()
    TArray<TObjectPtr<UMovePanel>> MovePanels;

    /**
     * The widget that acts as the cursor for the window
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> CursorWidget;

    /**
     * The current Pokémon whose moves are being displayed
     */
    UPROPERTY()
    TScriptInterface<IPokemon> CurrentPokemon;

    /**
     * Event for when the cursor moves to another move
     */
    UPROPERTY(BlueprintAssignable)
    FOnMoveSelectionChanged OnMoveSelectionChanged;
};
