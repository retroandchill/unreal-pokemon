// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonPanel.generated.h"

class UImage;
class UDisplayText;
class IPokemon;

/**
 * Widget for housing a single panel of a Pokémon
 */
UCLASS()
class POKEMONUI_API UPokemonPanel : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Get the Pokémon that this panel displays the information for
	 * @return The Pokémon that this panel displays the information for
	 */
	const TSharedPtr<IPokemon> &GetPokemon();
	
	/**
	 * Set the Pokémon for the panel setting how everything is displayed
	 * @param NewPokemon The Pokémon to set this panel to
	 */
	void SetPokemon(TSharedPtr<IPokemon> NewPokemon);

protected:
	/**
	 * Refresh the contents of the window setting the values where appropriate
	 */
	void Refresh();

private:
	/**
	 * Refresh all the information related to the Pokémon
	 */
	void RefreshPokemonInfo();
	
	/**
	 * The Pokémon reference that is currently being held onto
	 */
	TSharedPtr<IPokemon> Pokemon;

	/**
	 * The image that displays the icon of the Pokémon to the player
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PokemonIcon;

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
	 * The text block that displays the current and total HP of the Pokémon
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDisplayText> HPText;
};
