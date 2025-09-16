// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"

#include "PokemonSelectionPaneBase.generated.h"

class IPokemon;
class ISelectablePanel;
class UPartySelectCancelPanel;
class UWidgetSwitcher;
class UCanvasPanel;
class UUniformGridPanel;
class UPokemonPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPokemonPanelSelected, const TScriptInterface<IPokemon> &, Pokemon);

/**
 * Base widget for selecting a Pokémon from a list.
 */
UCLASS(Abstract, DisplayName = "Pokemon Selection Pane Base (Deprecated)")
class POKEMONUI_API UPokemonSelectionPaneBase : public USelectableWidget
{
    GENERATED_BODY()

  public:
    TOptional<UPokemonPanel &> FindPanelForPokemon(const TScriptInterface<IPokemon> &Pokemon) const;

    void SetPokemonToDisplay(TConstArrayView<TScriptInterface<IPokemon>> Pokemon);

    void RefreshWindow();

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

    void CancelSwitch();

    /**
     * Toggle whether the bottom commands should be visible or not
     * @param bIsVisible Should the elements be visible
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Display)
    void ToggleCommandVisibility(bool bIsVisible);

  protected:
    virtual TOptional<int32> GetNumPanelsToAdd() const;

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
    void SwitchPokemon(UPokemonPanel *Panel1, UPokemonPanel *Panel2);

  private:
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

    UPROPERTY()
    TArray<TObjectPtr<UWidget>> BlankPanels;

    /**
     * The index the player is switching from
     */
    TOptional<int32> SwitchingIndex;

    UPROPERTY(BlueprintAssignable, Category = Selection)
    FOnPokemonPanelSelected OnPokemonSelected;
};
