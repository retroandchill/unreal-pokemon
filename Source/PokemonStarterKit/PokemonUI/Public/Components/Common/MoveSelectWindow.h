// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"

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
class POKEMONUI_API UMoveSelectWindow : public USelectableWidget
{
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
    void NativeOnSelectionChange(int32 OldIndex, int32 NewIndex) override;

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
