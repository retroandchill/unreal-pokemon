// "Unreal Pokémon" created by Retro & Chill.
#include "Components/DisplayText.h"
#include "Components/SizeBox.h"

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
