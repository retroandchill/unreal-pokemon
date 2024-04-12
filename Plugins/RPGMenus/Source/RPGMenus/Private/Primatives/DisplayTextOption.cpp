// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/DisplayTextOption.h"

#include "Primatives/DisplayText.h"

void UDisplayTextOption::SetText(const FText& NewText) {
	DisplayText->SetText(NewText);
}

FVector2D UDisplayTextOption::GetTextSize() const {
	return DisplayText->GetTextSize();
}

FMargin UDisplayTextOption::GetDisplayTextPadding() const {
	return DisplayText->GetDisplayTextPadding();
}
