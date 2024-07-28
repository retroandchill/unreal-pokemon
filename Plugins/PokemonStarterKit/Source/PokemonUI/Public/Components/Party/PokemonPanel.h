// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "CommonButtonBase.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "SelectablePanel.h"

#include "PokemonPanel.generated.h"

class USelectableWidget;
class UPokemonSelectionPane;
class UProgressBar;
class UImage;
class UDisplayText;
class IPokemon;

/**
 * Widget for housing a single panel of a Pokémon
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UPokemonPanel : public UCommonButtonBase, public ISelectablePanel {
    GENERATED_BODY()

  public:
    void SetOwner(USelectableWidget *NewOwner) override;

    /**
     * Get the Pokémon that this panel displays the information for
     * @return The Pokémon that this panel displays the information for
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const TScriptInterface<IPokemon> &GetPokemon() const;

    /**
     * Set the Pokémon for the panel setting how everything is displayed
     * @param NewPokemon The Pokémon to set this panel to
     * @param Index The index of this particular panel
     */
    void SetPokemon(const TScriptInterface<IPokemon> &NewPokemon, int32 Index);

    /**
     * Swap held Pokémon with another panel
     * @param Other The other panel
     */
    void SwapPokemon(UPokemonPanel &Other);

    /**
     * Returns if this panel represents a Pokémon that is currently active in battle
     * @return Is this Pokémon currently active
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsActivated() const;

    /**
     * Returns if the current Pokémon is fainted or not?
     * @return Is the current Pokémon fainted?
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsPokemonFainted() const;

    /**
     * Returns if this panel represents the current index of the selection
     * @return Is this panel being selected?
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsPanelSelected() const;

    /**
     * Returns if this panel is the target of a Pokémon position switch
     * @return Is this Pokémon being considered for a swap out
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsSwapping() const;

    /**
     * Returns if this panel represents a Pokémon that is being swapped out
     * @return Is this Pokémon being swapped out
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsPreselected() const;

    UFUNCTION(BlueprintCallable, Category = Display)
    void Refresh() override;

  protected:
    /**
     * Set the Blueprint specific details of the widget based on the information from the state of the Pokémon
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
    void RefreshVisuals();

  private:
    /**
     * The panel class that owns this one
     */
    UPROPERTY()
    TObjectPtr<USelectableWidget> Owner;

    /**
     * The Pokémon reference that is currently being held onto
     */
    UPROPERTY(BlueprintGetter = GetPokemon, Category = Content)
    TScriptInterface<IPokemon> Pokemon;

    int32 PokemonIndex;
};
