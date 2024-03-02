// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/PokemonPanel.h"

#include "PokemonCore/Public/Pokemon/Pokemon.h"
#include "Primatives/DisplayText.h"
#include "Utilities/PokemonUIUtils.h"

/**
 * Set the text for the item if it is not null
 * @param TextWidget The item to set the text of
 * @param Text The text to set everything to
 */
void SetItemText(TObjectPtr<UDisplayText>& TextWidget, const FText &Text) {
	if (TextWidget == nullptr)
		return;

	TextWidget->SetText(Text);
}

/**
 * Set the text for the item if it is not null
 * @param TextWidget The item to set the text of
 * @param Text The text to set everything to
 */
void SetItemText(TObjectPtr<UDisplayText>& TextWidget, FStringView Text) {
	SetItemText(TextWidget, FText::FromStringView(Text));
}

const TSharedPtr<IPokemon>& UPokemonPanel::GetPokemon() {
	return Pokemon;
}

void UPokemonPanel::SetPokemon(TSharedPtr<IPokemon> NewPokemon) {
	Pokemon = MoveTemp(NewPokemon);
}

void UPokemonPanel::Refresh() {
	if (Pokemon != nullptr) {
		RefreshPokemonInfo();
	}
}

void UPokemonPanel::RefreshPokemonInfo() {
	SetItemText(NameText, Pokemon->GetName());
	SetItemText(LevelText, FString::FromInt(Pokemon->GetStatBlock().GetLevel()));

	// TODO: Change the text color depending on the gender
	switch (Pokemon->GetGender()) {
	case EGender::Male:
		SetItemText(GenderText, TEXT("♂"));
		break;
	case EGender::Female:
		SetItemText(GenderText, TEXT("♀"));
		break;
	case EGender::Genderless:
		SetItemText(GenderText, TEXT(""));
		break;
	}

	auto HP = FString::Format(TEXT("{CurrentHP} /{MaxHP}"), FStringFormatNamedArguments({
		{TEXT("CurrentHP"), UPokemonUIUtils::SpacePad(Pokemon->GetCurrentHP(), 3)},
		{TEXT("MaxHP"), UPokemonUIUtils::SpacePad(Pokemon->GetMaxHP(), 3)}
	}));

	SetItemText(HPText, HP);
}
