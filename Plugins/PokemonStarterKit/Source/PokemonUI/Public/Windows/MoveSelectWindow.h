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

    /**
     * Display the moves to the player.
     * @param Pokemon The Pokémon to display the moves for.
     * @param Move The move that is attempting to be taught
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    void BeginMoveLearnDisplay(const TScriptInterface<IPokemon> &Pokemon, FName Move);

    /**
     * Bind an action to when a move selection is changed
     * @param Callback The callback to when the selection is changed
     */
    void BindToOnMoveSelectionChanged(const FOnMoveSelectionChanged::FDelegate &Callback);

  protected:
    /**
     * Refresh the layout of this scene for if the Pokémon is trying to learn a move
     * @param bLearningMove Is the player trying to learn a move
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void RefreshLayout(bool bLearningMove);

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
     * @param bIsMoveToLearn
     * @return The created panel
     */
    UMovePanel *CreateMovePanel(const TScriptInterface<IMove> &Move, bool bIsMoveToLearn = false);

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
     * Optional move that is attempting to be taught to a Pokémon
     */
    UPROPERTY()
    TOptional<FName> MoveToLearn;

    /**
     * Event for when the cursor moves to another move
     */
    UPROPERTY(BlueprintAssignable)
    FOnMoveSelectionChanged OnMoveSelectionChanged;
};
