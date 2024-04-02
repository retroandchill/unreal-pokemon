// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/PokemonPanel.h"

#include "Components/Image.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Utilities/GraphicsLoadingSubsystem.h"
#include "Utilities/PokemonUIUtils.h"
#include "Windows/PokemonSelectionPane.h"
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

void UPokemonPanel::SwapPokemon(UPokemonPanel& Other) {
	Swap(Pokemon, Other.Pokemon);
}

bool UPokemonPanel::IsActive() const {
	return MenuIndex == 0;
}

bool UPokemonPanel::IsPokemonFainted() const {
	return Pokemon != nullptr ? Pokemon->IsFainted() : false;
}

bool UPokemonPanel::IsPanelSelected() const {
	return Owner != nullptr && Owner->GetIndex() == MenuIndex;
}

bool UPokemonPanel::IsSwapping() const {
	auto SelectionPane = Cast<UPokemonSelectionPane>(Owner);
	if (SelectionPane == nullptr)
		return false;
	
	return SelectionPane->IsSwitching() && IsPanelSelected();
}

bool UPokemonPanel::IsPreselected() const {
	auto SelectionPane = Cast<UPokemonSelectionPane>(Owner);
	if (SelectionPane == nullptr)
		return false;

	auto &SwitchingIndex = SelectionPane->GetSwitchingIndex();
	return SwitchingIndex.Get(-1) == MenuIndex;
}

void UPokemonPanel::Refresh() {
	RefreshVisuals();
	if (Pokemon != nullptr) {
		RefreshPokemonInfo();

		auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
		ASSERT(GraphicsLoadingSubsystem != nullptr)
		PokemonIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PokemonIcon->SetBrushResourceObject(GraphicsLoadingSubsystem->GetPokemonIcon(*Pokemon, this));
	} else {
		PokemonIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPokemonPanel::RefreshPokemonInfo() {
	UPokemonUIUtils::SetItemText(NameText, Pokemon->GetNickname());
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
