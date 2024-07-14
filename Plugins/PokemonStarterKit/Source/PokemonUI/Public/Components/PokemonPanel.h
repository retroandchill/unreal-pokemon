// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Bag/Item.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "SelectablePanel.h"

#include "PokemonPanel.generated.h"

class UCommonNumericTextBlock;
class UCommonTextBlock;
class USelectableWidget;
class UPokemonSelectionPane;
class UProgressBar;
class UImage;
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
    const TScriptInterface<IPokemon> &GetPokemon() const;

    /**
     * Set the Pokémon for the panel setting how everything is displayed
     * @param NewPokemon The Pokémon to set this panel to
     * @param Index The index of this particular panel
     */
    void SetPokemon(TScriptInterface<IPokemon> NewPokemon, int32 Index);

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

    UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
    UObject *GetHeldItemGraphic(const FItem &Item);

  private:
    /**
     * Refresh all the information related to the Pokémon
     */
    void RefreshPokemonInfo();

    /**
     * The panel class that owns this one
     */
    TObjectPtr<USelectableWidget> Owner;

    /**
     * The Pokémon reference that is currently being held onto
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    int32 PokemonIndex;

    /**
     * The image that displays the icon of the Pokémon to the player
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonIcon;

    /**
     * The image asset that forms the held item icon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> HeldItemIcon;

    /**
     * The text that displays the name for the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> NameText;

    /**
     * The text block that displays the level for the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonNumericTextBlock> LevelText;

    /**
     * The text block that displays the gender symbol for the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> GenderText;

    /**
     * The color settings for the gender text widget
     */
    UPROPERTY(EditAnywhere, Category = "Visuals|Text")
    TMap<EPokemonGender, TSubclassOf<UCommonTextStyle>> GenderTextColors;

    /**
     * The text block that displays the current and total HP of the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> HPText;

    /**
     * The image that displays the HP bar when necessary
     */
    UPROPERTY(DisplayName = "HP Bar", meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;
};
