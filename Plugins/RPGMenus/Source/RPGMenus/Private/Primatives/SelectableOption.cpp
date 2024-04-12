// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/SelectableOption.h"

#include "Components/Button.h"

void USelectableOption::NativeConstruct() {
	Super::NativeConstruct();
	PrimaryButton->OnClicked.AddDynamic(this, &USelectableOption::OnClicked);
	PrimaryButton->OnHovered.AddDynamic(this, &USelectableOption::OnHovered);
}

int32 USelectableOption::GetOptionIndex() const {
	return OptionIndex;
}

void USelectableOption::SetOptionIndex(int32 NewOptionIndex) {
	OptionIndex = NewOptionIndex;
}

FProcessButtonAction& USelectableOption::GetOnOptionClicked() {
	return OnOptionClicked;
}

FProcessButtonAction& USelectableOption::GetOnOptionHovered() {
	return OnOptionHovered;
}

void USelectableOption::OnClicked() {
	OnOptionClicked.Broadcast(this);
}

void USelectableOption::OnHovered() {
	OnOptionHovered.Broadcast(this);
}
