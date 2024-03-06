// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Refreshable.h"
#include "SelectablePanel.h"
#include "Blueprint/UserWidget.h"
#include "Pokemon/Breeding/Gender.h"
#include "Text/TextColor.h"
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
UCLASS()
class POKEMONUI_API UPokemonPanel : public UUserWidget, public ISelectablePanel {
	GENERATED_BODY()

public:
	void SetOwner(USelectableWidget* NewOwner) override;
	
	/**
	 * Get the Pokémon that this panel displays the information for
	 * @return The Pokémon that this panel displays the information for
	 */
	const TSharedPtr<IPokemon> &GetPokemon();
	
	/**
	 * Set the Pokémon for the panel setting how everything is displayed
	 * @param NewPokemon The Pokémon to set this panel to
	 * @param Index The index of this particular panel
	 */
	void SetPokemon(TSharedPtr<IPokemon> NewPokemon, int32 Index);

	/**
	* Returns if this panel represents a Pokémon that is currently active in battle
	 * @return Is this Pokémon currently active
	 */
	UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
	bool IsActive() const;

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
	
	void Refresh() override;

protected:
	/**
	 * Set the Blueprint specific details of the widget based on the information from the state of the Pokémon
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
	void RefreshVisuals();

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
	TSharedPtr<IPokemon> Pokemon;

	/**
	 * The index of this particular panel
	 */
	int32 MenuIndex;

	/**
	 * The image that displays the icon of the Pokémon to the player
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PokemonIcon;

	/**
	 * The image asset that forms the held item icon
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> HeldItemIcon;

	/**
	 * The text that displays the name for the Pokémon
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDisplayText> NameText;

	/**
	 * The text block that displays the level for the Pokémon
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDisplayText> LevelText;

	/**
	 * The text block that displays the gender symbol for the Pokémon
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDisplayText> GenderText;

	/**
	 * The color settings for the gender text widget
	 */
	UPROPERTY(EditAnywhere, Category = "Visuals|Text")
	TMap<EGender, FTextColor> GenderTextColors;

	/**
	 * The text block that displays the current and total HP of the Pokémon
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDisplayText> HPText;

	/**
	 * The image that displays the HP bar when necessary
	 */
	UPROPERTY(DisplayName = "HP Bar", meta=(BindWidget))
	TObjectPtr<UProgressBar> HPBar;
};
