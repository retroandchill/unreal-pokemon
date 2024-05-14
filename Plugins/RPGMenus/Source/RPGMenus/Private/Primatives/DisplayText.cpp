// "Unreal Pokémon" created by Retro & Chill.
#include "Primatives/DisplayText.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Data/RPGMenusSettings.h"
#include "Fonts/FontMeasure.h"
#include "Utilities/WidgetUtilities.h"

UDisplayText::UDisplayText(const FObjectInitializer &ObjectInitializer)
    : UUserWidget(ObjectInitializer), TextStyles(GetDefault<URPGMenusSettings>()->GetTextStyleDataTable()) {
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
        DisplayTextWidget->SetTextStyleSet(TextStyles);
        DisplayTextWidget->SetText(InitialText);

        DisplayTextWidget->ClearAllDefaultStyleOverrides();
        auto DefaultTextStyle = DisplayTextWidget->GetDefaultTextStyle();
        bool bStyleChanged = false;
        if (DisplayFont.IsSet()) {
            DefaultTextStyle.Font = DisplayFont.GetValue();
            bStyleChanged = true;
        }

        if (TextColor.IsSet()) {
            DefaultTextStyle.ColorAndOpacity = TextColor.GetValue();
            bStyleChanged = true;
        }

        if (ShadowColor.IsSet()) {
            DefaultTextStyle.ShadowColorAndOpacity = ShadowColor.GetValue().GetSpecifiedColor();
            bStyleChanged = true;
        }

        if (bStyleChanged) {
            DisplayTextWidget->SetDefaultTextStyle(DefaultTextStyle);
        }

        if (SizeBox != nullptr) {
            auto TextPadding = GetDisplayTextPadding();
            SizeBox->SetHeightOverride(static_cast<float>(GetTextSize().Y) + TextPadding.Top + TextPadding.Bottom);
        }
    }
}

FText UDisplayText::GetText() const {
    check(DisplayTextWidget != nullptr)
    return DisplayTextWidget->GetText();
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

void UDisplayText::SetTextColor(const FSlateColor &Color) {
    TextColor = Color;
    SetTextInfo();
}

void UDisplayText::SetShadowColor(const FSlateColor &Color) {
    ShadowColor = Color;
    SetTextInfo();
}

FVector2D UDisplayText::GetTextSize() const {
    check(DisplayTextWidget != nullptr)
    return GetTextSize(DisplayTextWidget->GetText().ToString());
}

FVector2D UDisplayText::GetTextSize(const FString &Text) const {
    check(DisplayTextWidget != nullptr)
    auto FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
    FVector2D Size = FontMeasure->Measure(Text, DisplayTextWidget->GetCurrentDefaultTextStyle().Font,
                                          UWidgetUtilities::GetWidgetDPIScale());
    return Size;
}

FVector2D UDisplayText::GetTotalTextAreaSize() const {
    check(DisplayTextWidget != nullptr)
    return DisplayTextWidget->GetCachedGeometry().GetLocalSize();
}

void UDisplayText::OnTextSet_Implementation(const FText &Text) {
    // No definition needed here in this class
}

URichTextBlock *UDisplayText::GetDisplayTextWidget() const {
    return DisplayTextWidget;
}
