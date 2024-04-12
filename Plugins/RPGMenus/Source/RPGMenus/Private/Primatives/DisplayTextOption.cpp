// "Unreal Pokémon" created by Retro & Chill.


#include "Primatives/DisplayTextOption.h"

#include "Primatives/DisplayText.h"

FText UDisplayTextOption::GetText() const {
	return DisplayText->GetText();
}

void UDisplayTextOption::SetText(const FText& NewText) {
	DisplayText->SetText(NewText);
}

const FSlateFontInfo& UDisplayTextOption::GetDisplayFont() const {
	return DisplayText->GetDisplayFont();
}

void UDisplayTextOption::SetTextColor(const FSlateColor& Color) {
	DisplayText->SetTextColor(Color);
}

FVector2D UDisplayTextOption::GetTextSize() const {
	return DisplayText->GetTextSize();
}

FVector2D UDisplayTextOption::GetTextSize(const FString& Text) const {
	return DisplayText->GetTextSize(Text);
}

FVector2D UDisplayTextOption::GetTotalTextAreaSize() const {
	return DisplayText->GetTotalTextAreaSize();
}

FMargin UDisplayTextOption::GetDisplayTextPadding() const {
	return DisplayText->GetDisplayTextPadding();
}
