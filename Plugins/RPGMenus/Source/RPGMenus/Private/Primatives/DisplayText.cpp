// "Unreal Pokémon" created by Retro & Chill.
#include "Primatives/DisplayText.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Fonts/FontMeasure.h"
#include "Utilities/WidgetUtilities.h"

UDisplayText::UDisplayText(const FObjectInitializer &ObjectInitializer) : UUserWidget(ObjectInitializer) {
}

TSharedRef<SWidget> UDisplayText::RebuildWidget() {
    auto Ret = Super::RebuildWidget();
    SetTextInfo();
    return Ret;
}

void UDisplayText::SynchronizeProperties() {
    Super::SynchronizeProperties();
    SetTextInfo();
}

void UDisplayText::SetTextInfo() {
    if (DisplayTextWidget != nullptr) {
        DisplayTextWidget->SetText(InitialText);
        DisplayTextWidget->SetFont(DisplayFont);
        DisplayTextWidget->SetColorAndOpacity(TextColor);

        if (SizeBox != nullptr) {
            auto TextPadding = GetDisplayTextPadding();
            SizeBox->SetHeightOverride(static_cast<float>(GetTextSize().Y) + TextPadding.Top + TextPadding.Bottom);
        }
    }
}

FText UDisplayText::GetText() const {
    check(DisplayTextWidget != nullptr) return DisplayTextWidget->GetText();
}

void UDisplayText::SetText(const FText &NewText) {
    InitialText = NewText;
    if (DisplayTextWidget != nullptr) {
        DisplayTextWidget->SetText(NewText);

        if (SizeBox != nullptr) {
            auto TextPadding = GetDisplayTextPadding();
            SizeBox->SetHeightOverride(static_cast<float>(GetTextSize().Y) + TextPadding.Top + TextPadding.Bottom);
        }

        OnTextSet(NewText);
    }
}

const FSlateFontInfo &UDisplayText::GetDisplayFont() const {
    return DisplayFont;
}

void UDisplayText::SetTextColor(const FSlateColor &Color) {
    TextColor = Color;

    check(DisplayTextWidget != nullptr) DisplayTextWidget->SetColorAndOpacity(TextColor);
}

FVector2D UDisplayText::GetTextSize() const {
    check(DisplayTextWidget != nullptr) return GetTextSize(DisplayTextWidget->GetText().ToString());
}

FVector2D UDisplayText::GetTextSize(const FString &Text) const {
    check(DisplayTextWidget != nullptr) auto FontMeasure =
        FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
    FVector2D Size = FontMeasure->Measure(Text, DisplayFont, UWidgetUtilities::GetWidgetDPIScale());
    return Size;
}

FVector2D UDisplayText::GetTotalTextAreaSize() const {
    check(DisplayTextWidget != nullptr) return DisplayTextWidget->GetCachedGeometry().GetLocalSize();
}

void UDisplayText::OnTextSet_Implementation(const FText &Text) {
    // No definition needed here in this class
}

UTextBlock *UDisplayText::GetDisplayTextWidget() const {
    return DisplayTextWidget;
}
