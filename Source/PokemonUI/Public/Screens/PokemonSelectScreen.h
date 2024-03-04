// "Unreal Pokémon" created by Retro & Chill.
#pragma once


#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "PokemonSelectScreen.generated.h"

class UPokemonSelectionPane;
/**
 * Screen for when the player needs to select a Pokémon from the menu
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UPokemonSelectScreen : public UScreen {
	GENERATED_BODY()

public:

private:
	/**
	 * The "Window" that the player selects a Pokémon from
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPokemonSelectionPane> SelectionPane;
};
