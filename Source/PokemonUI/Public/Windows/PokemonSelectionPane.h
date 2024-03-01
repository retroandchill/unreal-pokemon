// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"
#include "PokemonSelectionPane.generated.h"

class UPokemonPanel;
/**
 * Pane used to select Pokémon from in a grid based format
 */
UCLASS()
class POKEMONUI_API UPokemonSelectionPane : public USelectableWidget {
	GENERATED_BODY()

public:

private:
	/**
	 * The class used for the panels that house the 6 Pokémon
	 */
	UPROPERTY(EditAnywhere, Category = Display)
	TSubclassOf<UPokemonPanel> PanelClass;

	/**
	 * How many columns will the Pokémon be displayed in?
	 */
	UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin =1))
	int32 Columns = 2;
};
