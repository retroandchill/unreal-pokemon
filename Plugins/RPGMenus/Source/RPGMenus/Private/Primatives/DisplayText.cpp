// "Unreal Pokémon" created by Retro & Chill.
#include "Primatives/DisplayText.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Data/RPGMenusSettings.h"
#include "Fonts/FontMeasure.h"
#include "Utilities/WidgetUtilities.h"

void UDisplayText::NativePreConstruct() {
    Super::NativePreConstruct();
    OnTextSet(Text);
    OnTextStyleSet(TextStyle);
}

FText UDisplayText::GetText() const {
    return Text;
}

void UDisplayText::SetText(const FText &NewText) {
    Text = NewText;
    OnTextSet(Text);
}

TSubclassOf<UCommonTextStyle> UDisplayText::GetTextStyle() const {
    return TextStyle;
}

void UDisplayText::SetTextStyle(TSubclassOf<UCommonTextStyle> NewStyle) {
    TextStyle = NewStyle;
    OnTextStyleSet(TextStyle);
}
