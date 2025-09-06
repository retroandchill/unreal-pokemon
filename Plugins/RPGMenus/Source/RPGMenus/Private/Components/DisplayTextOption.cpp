// "Unreal Pokémon" created by Retro & Chill.

#include "Components/DisplayTextOption.h"
#include "CommonTextBlock.h"

void UDisplayTextOption::NativePreConstruct()
{
    Super::NativePreConstruct();
    DisplayText->SetText(Text);
}

const FText &UDisplayTextOption::GetText() const
{
    return Text;
}

void UDisplayTextOption::SetText(const FText &NewText)
{
    Text = NewText;
    DisplayText->SetText(Text);
}

void UDisplayTextOption::NativeOnCurrentTextStyleChanged()
{
    Super::NativeOnCurrentTextStyleChanged();
    if (DisplayText != nullptr)
    {
        DisplayText->SetStyle(GetCurrentTextStyleClass());
    }
}