// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/PokemonBattlePanel.h"
#include "Screens/Screen.h"

#include "PokemonBattleScreen.generated.h"

class IBattleMove;
class UBattleMoveSelect;
class UPokemonActionOptions;
class IBattleSide;
class IBattle;

/**
 * The underlying screen that sits on top of the battle system.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonBattleScreen : public UScreen {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;
    
  public:
    /**
     * Set the battle that this screen holds
     * @param Battle The battle in question
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void SetBattle(const TScriptInterface<IBattle> &Battle);

    /**
     * Have a battler select an action from the HUD.
     * @param Battler The battler who is selecting an action.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void SelectAction(const TScriptInterface<IBattler>& Battler);

    /**
     * Get the action select widget 
     * @return Get the action select widget
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Battle|Selection")
    UPokemonActionOptions* GetActionSelect() const;

    void SelectMove(const TScriptInterface<IBattler>& Battler);

    /**
     * Refresh the battle HUD
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void Refresh() const;

  protected:
    /**
     * Slot a panel into the widget
     * @param Panel The panel being slotted
     * @param Side The side of battle that the panel is for (0 generally means the player, while 1 is the opponent)
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals", meta = (ForceAsFunctions))
    void SlotPanel(UPokemonBattlePanel *Panel, int32 Side);

  private:
    /**
     * Add panels for a given side
     * @param Index The index of the side
     * @param Side The actual side itself
     */
    void AddPanelsForSide(int32 Index, const TScriptInterface<IBattleSide> &Side);

    /**
     * Create the actual battle panel and slot it into the widget
     * @param Side The side of battle that we're adding to
     * @param Battler The battler to create the panel for
     */
    void CreateBattlePanel(int32 Side, const TScriptInterface<IBattler> &Battler);

    /**
     * Called when the fight command is selected
     * @param Index The selected index
     */
    UFUNCTION()
    void OnActionSelected(int32 Index);

    /**
     * Called when a move is selected on a battler
     * @param Battler The battler to use the move
     * @param Move The selected move
     */
    UFUNCTION()
    void OnMoveSelected(const TScriptInterface<IBattler>& Battler, const TScriptInterface<IBattleMove>& Move);

    /**
     * Called when the player cancels move selection
     */
    UFUNCTION()
    void OnMoveCanceled();

    /**
     * The widget that is used to select the options from the menu
     */
    UPROPERTY(BlueprintGetter = GetActionSelect, Category = "Widgets", meta = (BindWidget))
    TObjectPtr<UPokemonActionOptions> ActionSelect;

    /**
     * The widget that is used to select moves from
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBattleMoveSelect> MoveSelect;
    
    /**
     * The battle that this screen is showing the information for
     */
    UPROPERTY()
    TScriptInterface<IBattle> CurrentBattle;

    /**
     * The list of panel classes to use for each side of battle. If there is not one for a given side, the game
     * will crash so make sure this is set correctly.
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Visuals")
    TArray<TSoftClassPtr<UPokemonBattlePanel>> PanelClasses;
    

    /**
     * The panels that were dynamically added to the widget
     */
    UPROPERTY()
    TArray<TObjectPtr<UPokemonBattlePanel>> Panels;

    /**
     * A list of battlers who are currently selecting actions.
     */
    UPROPERTY()
    TArray<TScriptInterface<IBattler>> SelectingBattlers;

    /**
     * The index in the selection list we are currently on.
     */
    UPROPERTY()
    TOptional<int32> SelectionIndex;
};
