// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/PokemonBattlePanel.h"
#include "Screens/Screen.h"

#include "PokemonBattleScreen.generated.h"

class UBattleSwitchPane;
struct FExpGainInfo;
class UExpGainPane;
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
     * Get the current battle for this screen
     * @return The current battle for this screen
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Battle|Visuals")
    const TScriptInterface<IBattle> &GetBattle() const;
    
    /**
     * Set the battle that this screen holds
     * @param Battle The battle in question
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void SetBattle(const TScriptInterface<IBattle> &Battle);

    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void ClearSelectingBattlers();

    /**
     * Have a battler select an action from the HUD.
     * @param Battler The battler who is selecting an action.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void SelectAction(const TScriptInterface<IBattler> &Battler);

    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void PromptMandatorySwitch(const TScriptInterface<IBattler> &Battler);

    /**
     * Get the action select widget
     * @return Get the action select widget
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Widgets")
    UPokemonActionOptions *GetActionSelect() const;

    /**
     * Get the switch selection pane widget
     * @return The pane used to select Pokémon to switch in
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Widgets")
    UBattleSwitchPane *GetBattleSwitchPane() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Widgets")
    UExpGainPane *GetExpGainPane() const;

    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void SelectMove(const TScriptInterface<IBattler> &Battler);

    /**
     * Refresh the battle HUD
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void Refresh() const;

    /**
     * Attempt to find the panel for a battler
     * @param Battler The battler so check for the panel of
     * @return The found panel
     */
    UPokemonBattlePanel *FindPanelForBattler(const TScriptInterface<IBattler> &Battler) const;

    void DisplayExpForGain(TArray<FExpGainInfo> &&GainInfos);

    FDelegateHandle BindToExpGainComplete(FSimpleDelegate &&Callback);

    void RemoveFromExpGainComplete(FDelegateHandle Handle);

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void FinishExpGainDisplay();

  protected:
    /**
     * Slot a panel into the widget
     * @param Panel The panel being slotted
     * @param Side The side of battle that the panel is for (0 generally means the player, while 1 is the opponent)
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void SlotPanel(UPokemonBattlePanel *Panel, int32 Side);

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void SwapToPanelDisplay();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void OnMandatorySwitch();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void SwapToExpGainDisplay();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void PlayExpGainAnimation();

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
     * Advance to the next battler in the window
     * @param Battler The battler to move on to
     */
    void NextBattler(const TScriptInterface<IBattler> &Battler);

    void AdvanceToNextSelection();

  protected:
    /**
     * Called when a move is selected on a battler
     * @param Battler The battler to use the move
     * @param Move The selected move
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void OnMoveSelected(const TScriptInterface<IBattler> &Battler, const TScriptInterface<IBattleMove> &Move);

    /**
     * Skip the actions of all remaining battlers in line.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void SkipRemainingActions();

    /**
     * Called when the player cancels move selection
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void OnMoveCanceled();

    UFUNCTION(BlueprintCallable, Category = "Battle|Selection")
    void OnSwitchSelected(const TScriptInterface<IBattler> &Battler, const TScriptInterface<IBattler> &Target);

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Selection")
    void HideSwitchWindow();

    UFUNCTION(BlueprintCallable, Category = "Battle|Exp")
    void CompleteExpGain();

  private:
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
     * The pane used to select Pokémon to switch in
     */
    UPROPERTY(BlueprintGetter = GetBattleSwitchPane, Category = "Widgets", meta = (BindWidget))
    TObjectPtr<UBattleSwitchPane> BattleSwitchPane;

    UPROPERTY(BlueprintGetter = GetExpGainPane, Category = "Widgets", meta = (BindWidget))
    TObjectPtr<UExpGainPane> ExpGainPane;

    /**
     * The battle that this screen is showing the information for
     */
    UPROPERTY(BlueprintGetter = GetBattle, Category = "Battle|Visuals")
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

    /**
     * Called when the Exp. Gain Animation and move learning is complete
     */
    FSimpleMulticastDelegate OnExpGainComplete;

    bool bMandatorySwitch = false;
};
