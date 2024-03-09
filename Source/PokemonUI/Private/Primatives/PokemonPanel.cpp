// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/PokemonPanel.h"

#include "Components/Image.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/GraphicsLoadingSubsystem.h"
#include "Utilities/PokemonUIUtils.h"
#include "Windows/SelectableWidget.h"

void UPokemonPanel::SetOwner(USelectableWidget* NewOwner) {
	Owner = NewOwner;
}

const TSharedPtr<IPokemon>& UPokemonPanel::GetPokemon() {
	return Pokemon;
}

void UPokemonPanel::SetPokemon(TSharedPtr<IPokemon> NewPokemon, int32 Index) {
	Pokemon = MoveTemp(NewPokemon);
	MenuIndex = Index;
	Refresh();
}

bool UPokemonPanel::IsActive() const {
	// TODO: There's a bit more to this I think, but for now this will work
	return MenuIndex == 0;
}

bool UPokemonPanel::IsPokemonFainted() const {
	return Pokemon != nullptr ? Pokemon->IsFainted() : false;
}

bool UPokemonPanel::IsPanelSelected() const {
	return Owner != nullptr && Owner->GetIndex() == MenuIndex;
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

		auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
		check(GraphicsLoadingSubsystem != nullptr)
		PokemonIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PokemonIcon->SetBrushResourceObject(GraphicsLoadingSubsystem->GetPokemonIcon(*Pokemon));
	} else {
		PokemonIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void UPokemonPanel::RefreshPokemonInfo() {
	UPokemonUIUtils::SetItemText(NameText, Pokemon->GetName());
	UPokemonUIUtils::SetItemText(LevelText, FString::FromInt(Pokemon->GetStatBlock().GetLevel()));

	// TODO: Change the text color depending on the gender
	auto Gender = Pokemon->GetGender();
	switch (Gender) {
	case EPokemonGender::Male:
		UPokemonUIUtils::SetItemText(GenderText, TEXT("♂"));
		break;
	case EPokemonGender::Female:
		UPokemonUIUtils::SetItemText(GenderText, TEXT("♀"));
		break;
	case EPokemonGender::Genderless:
		UPokemonUIUtils::SetItemText(GenderText, TEXT(""));
		break;
	}
	
	if (GenderTextColors.Contains(Gender)) {
		UPokemonUIUtils::SetItemTextColor(GenderText, GenderTextColors[Gender]);
	}

	auto HP = FString::Format(TEXT("{CurrentHP} /{MaxHP}"), FStringFormatNamedArguments({
		{TEXT("CurrentHP"), UPokemonUIUtils::SpacePad(Pokemon->GetCurrentHP(), 3)},
		{TEXT("MaxHP"), UPokemonUIUtils::SpacePad(Pokemon->GetMaxHP(), 3)}
	}));

	UPokemonUIUtils::SetItemText(HPText, HP);
	UPokemonUIUtils::SetBarValues(HPBar, Pokemon->GetCurrentHP(), Pokemon->GetMaxHP());
}
