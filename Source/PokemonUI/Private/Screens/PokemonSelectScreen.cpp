// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PokemonSelectScreen.h"

#include "Windows/PokemonSelectionPane.h"

void UPokemonSelectScreen::NativeConstruct() {
	Super::NativeConstruct();
	check(SelectionPane != nullptr);
	SelectionPane->SetIndex(0);
	SelectionPane->SetActive(true);
}
