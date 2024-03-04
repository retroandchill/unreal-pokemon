// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/PokemonPanel.h"

#include "PokemonCore/Public/Pokemon/Pokemon.h"
#include "Primatives/PokemonPanelState.h"
#include "Utilities/PokemonUIUtils.h"

const TSharedPtr<IPokemon>& UPokemonPanel::GetPokemon() {
	return Pokemon;
}

void UPokemonPanel::SetPokemon(TSharedPtr<IPokemon> NewPokemon) {
	Pokemon = MoveTemp(NewPokemon);
}

bool UPokemonPanel::IsActive() const {
	// TODO: Actually implement the functionality for this
	return false;
}

bool UPokemonPanel::IsPokemonFainted() const {
	return Pokemon != nullptr ? Pokemon->IsFainted() : false;
}

bool UPokemonPanel::IsPanelSelected() const {
	// TODO: Actually implement the functionality for this
	return false;
}

bool UPokemonPanel::IsSwapping() const {
	// TODO: Actually implement the functionality for this
	return false;
}

bool UPokemonPanel::IsPreselected() const {
	// TODO: Actually implement the functionality for this
	return false;
}

void UPokemonPanel::Refresh() {
	RefreshVisuals();
	if (Pokemon != nullptr) {
		RefreshPokemonInfo();
	}
	
}

void UPokemonPanel::RefreshPokemonInfo() {
	UPokemonUIUtils::SetItemText(NameText, Pokemon->GetName());
	UPokemonUIUtils::SetItemText(LevelText, FString::FromInt(Pokemon->GetStatBlock().GetLevel()));

	// TODO: Change the text color depending on the gender
	switch (Pokemon->GetGender()) {
	case EGender::Male:
		UPokemonUIUtils::SetItemText(GenderText, TEXT("♂"));
		break;
	case EGender::Female:
		UPokemonUIUtils::SetItemText(GenderText, TEXT("♀"));
		break;
	case EGender::Genderless:
		UPokemonUIUtils::SetItemText(GenderText, TEXT(""));
		break;
	}

	auto HP = FString::Format(TEXT("{CurrentHP} /{MaxHP}"), FStringFormatNamedArguments({
		{TEXT("CurrentHP"), UPokemonUIUtils::SpacePad(Pokemon->GetCurrentHP(), 3)},
		{TEXT("MaxHP"), UPokemonUIUtils::SpacePad(Pokemon->GetMaxHP(), 3)}
	}));

	UPokemonUIUtils::SetItemText(HPText, HP);
	UPokemonUIUtils::SetBarValues(HPBar, Pokemon->GetCurrentHP(), Pokemon->GetMaxHP());
}
