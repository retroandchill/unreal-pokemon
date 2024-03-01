// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonPanel.generated.h"

class IPokemon;

/**
 * Widget for housing a single panel of a Pokémon
 */
UCLASS()
class POKEMONUI_API UPokemonPanel : public UUserWidget {
	GENERATED_BODY()

public:
	

private:
	/**
	 * The Pokémon reference that is currently being held onto
	 */
	TSharedPtr<IPokemon> Pokemon;
};
