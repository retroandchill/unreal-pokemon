// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Primatives/PokemonPanel.h"
#include "Windows/SelectableWidget.h"
#include "PokemonSelectionPane.generated.h"

class ISelectablePanel;
class UPartySelectCancelPanel;
class UWidgetSwitcher;
class UCanvasPanel;
class UUniformGridPanel;
class UPokemonPanel;

/**
 * Pane used to select Pokémon from in a grid based format
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UPokemonSelectionPane : public USelectableWidget {
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	int32 GetItemCount_Implementation() const override;
	int32 GetColumnCount_Implementation() const override;

	/**
	 * Is this panel currently in multi-select mode?
	 * @return Is this panel in single select mode or multi-select mode?
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Display)
	bool IsMultiSelectMode() const;

	/**
	 * Is the player actively switching Pokémon
	 * @return The current switching state
	 */
	UFUNCTION(BlueprintPure, Category = Switching)
	bool IsSwitching() const;

	/**
	 * Get if the window is actively switching Pokémon and if so what the index is
	 * @return Is current switching index (if switching)
	 */
	const TOptional<int32> &GetSwitchingIndex() const;

	/**
	 * Begin the process of switching
	 * @param StartIndex The index to start the switch at
	 */
	UFUNCTION(BlueprintCallable, Category = Switching)
	void BeginSwitch(int32 StartIndex);

	/**
	 * End the process of switching
	 */
	UFUNCTION(BlueprintCallable, Category = Switching)
	void CompleteSwitch();

	/**
	 * Toggle whether the bottom commands should be visible or not
	 * @param bIsVisible Should the elements be visible
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Display)
	void ToggleCommandVisibility(bool bIsVisible);

protected:
	/**
	 * Get the location in the grid to place the panel in
	 * @param PanelIndex The index of the new panel
	 * @param Offsets The positioning to place the panel in
	 * @param Anchors The percentage anchors used to place the panel
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Placement")
	void GetPanelOffset(int32 PanelIndex, FMargin &Offsets, FAnchors &Anchors);

	/**
	 * Get the location in the grid to place the panel in
	 * @param PanelIndex The index of the new panel
	 * @return The positioning to place the panel in and the percentage anchors used to place the panel
	 */
	TPair<FMargin, FAnchors> GetPanelOffset(int32 PanelIndex);

	void OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) override;

	/**
	 * Perform a swap with the two panels being the targets
	 * @param Panel1 The first panel in the swap
	 * @param Panel2 The second panel in the swap
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Switching)
	void PerformSwap(UPokemonPanel *Panel1, UPokemonPanel *Panel2);

	/**
	 * Actually swap the Pokémon on the panels
	 * @param Panel1 The first panel in the swap
	 * @param Panel2 The second panel in the swap
	 */
	UFUNCTION(BlueprintCallable, Category = Switching)
	static void SwitchPokemon(UPokemonPanel *Panel1, UPokemonPanel *Panel2);

private:
	/**
	 * Convenience method to add a panel to the window and set the index.
	 * @param Panel The panel to add.
	 */
	void AddAdditionalPanelToOptions(TObjectPtr<UPartySelectCancelPanel>& Panel);

	/**
	 * The canvas to place the panels in
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ContentsArea;

	/**
	 * The switcher used to toggle the buttons shown to the player when in multi-select mode.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ConfirmCancelSwitcher;

	/**
	 * The switcher used to toggle the buttons shown to the player when in multi-select mode.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPartySelectCancelPanel> CancelPanel;

	/**
	 * The switcher used to toggle the buttons shown to the player when in multi-select mode.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPartySelectCancelPanel> MultiSelectConfirmPanel;

	/**
	 * The switcher used to toggle the buttons shown to the player when in multi-select mode.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPartySelectCancelPanel> MultiSelectCancelPanel;

	/**
	 * The class used for the panels that house the 6 Pokémon
	 */
	UPROPERTY(EditAnywhere, Category = Display)
	TSubclassOf<UPokemonPanel> PanelClass;

	/**
	 * The class used for the blank panels used to fill space
	 */
	UPROPERTY(EditAnywhere, Category = Display)
	TSubclassOf<UWidget> BlankPanelClass;

	/**
	 * The list of panels being displayed by this widget
	 */
	UPROPERTY()
	TArray<TScriptInterface<ISelectablePanel>> ActivePanels;

	/**
	 * How many columns will the Pokémon be displayed in?
	 */
	UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin =1))
	int32 Columns = 2;

	/**
	 * Is this panel being used to select multiple Pokémon or just one?
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=IsMultiSelectMode, Category = Display)
	bool bMultiSelectMode = false;

	/**
	 * The index the player is switching from
	 */
	TOptional<int32> SwitchingIndex;
};
