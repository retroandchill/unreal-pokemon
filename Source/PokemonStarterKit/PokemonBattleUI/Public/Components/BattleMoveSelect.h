// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"

#include "BattleMoveSelect.generated.h"

class UPokemonBattleScreen;
class IBattle;
class IBattleMove;
class UBattleMovePanel;
class IBattler;
class UMovePanel;

/**
 * Dynamic delegate for when a move is selected from the window.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveSelected, const TScriptInterface<IBattler> &, Battler,
                                             const TScriptInterface<IBattleMove> &, Move);

/**
 * Selection window for picking moves to use in combat.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleMoveSelect : public USelectableWidget
{
    GENERATED_BODY()

  public:
    /**
     * Set the battler to show the moves for
     * @param NewBattler The battler to show the moves for
     */
    void SetBattler(const TScriptInterface<IBattler> &NewBattler);

    /**
     * Get the move selection delegate
     * @return Delegate called when a move is selected
     */
    FOnMoveSelected &GetOnMoveSelected();

  protected:
    void NativeProcessConfirm(int32 CurrentIndex) override;

  private:
    /**
     * Create a move panel for the given move
     * @param Move The move in question
     */
    void CreateMovePanel(const TScriptInterface<IBattleMove> &Move);

    /**
     * The battler that sits underneath this one
     */
    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    /**
     * The subclass of move panel used to select
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Selection")
    TSubclassOf<UBattleMovePanel> MovePanelClass;

    /**
     * Delegate called when a move is selected
     */
    UPROPERTY(BlueprintAssignable)
    FOnMoveSelected OnMoveSelected;
};
