// "Unreal Pokémon" created by Retro & Chill.
#include "Text/ShadowedText.h"

#include "Asserts.h"
#include "Components/TextBlock.h"

void UShadowedText::SetTextInfo() {
	Super::SetTextInfo();
	SetShadowTextAndFont(Shadow1);
	SetShadowTextAndFont(Shadow2);
	SetShadowTextAndFont(Shadow3);
}

void UShadowedText::OnTextSet_Implementation(const FText& Text) {
	SetShadowText(Shadow1, Text);
	SetShadowText(Shadow2, Text);
	SetShadowText(Shadow3, Text);
}

UTextBlock* UShadowedText::GetShadow1() const {
	return Shadow1;
}

UTextBlock* UShadowedText::GetShadow2() const {
	return Shadow2;
}

UTextBlock* UShadowedText::GetShadow3() const {
	return Shadow3;
}

void UShadowedText::SetShadowColor(const FSlateColor& Color) {
	ShadowColor = Color;
	SetShadowTextAndFont(Shadow1);
	SetShadowTextAndFont(Shadow2);
	SetShadowTextAndFont(Shadow3);
}

void UShadowedText::SetShadowTextAndFont(UTextBlock* Shadow) {
	if (Shadow != nullptr) {
		Shadow->SetText(GetText());
		Shadow->SetFont(GetDisplayFont());
		Shadow->SetColorAndOpacity(ShadowColor);
	}
}

void UShadowedText::SetShadowText(UTextBlock* Shadow, const FText& Text) {
	ASSERT(Shadow != nullptr)
	Shadow->SetText(Text);
}
