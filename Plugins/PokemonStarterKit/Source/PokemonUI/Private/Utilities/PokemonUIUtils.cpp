// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonUIUtils.h"
#include "PokemonMenuBase.h"
#include "PrimaryGameLayout.h"
#include "Components/ProgressBar.h"
#include "Pokemon/Pokemon.h"
#include "Primatives/DisplayText.h"
#include "Text/TextColor.h"

enum class EPokemonGender : uint8;

void UPokemonUIUtils::RemoveMenuFromStack(const UObject *WorldContextObject, UPokemonMenuBase *Menu) {
    UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject)->FindAndRemoveWidgetFromLayer(Menu);
}

FString UPokemonUIUtils::ZeroPad(int32 Value, int32 DesiredLength) {
    return PadInt(Value, DesiredLength);
}

FString UPokemonUIUtils::SpacePad(int32 Value, int32 DesiredLength) {
    return PadInt(Value, DesiredLength, TEXT(' '));
}

FString UPokemonUIUtils::PadInt(int32 Value, int32 DesiredLength, TCHAR PaddingCharacter) {
    auto AsString = FString::FromInt(Value);
    int32 Diff = DesiredLength - AsString.Len();
    if (Diff <= 0)
        return AsString;

    FString Output(TEXT(""), FMath::Max(DesiredLength, AsString.Len()));
    for (int i = 0; i < Diff; i++) {
        Output.AppendChar(PaddingCharacter);
    }

    Output.Append(AsString);
    return Output;
}

void UPokemonUIUtils::SetItemText(TObjectPtr<UDisplayText> &TextWidget, const FText &Text) {
    if (TextWidget == nullptr)
        return;

    TextWidget->SetText(Text);
}

void UPokemonUIUtils::SetItemText(TObjectPtr<UDisplayText> &TextWidget, FStringView Text) {
    SetItemText(TextWidget, FText::FromStringView(Text));
}

void UPokemonUIUtils::SetItemTextColor(TObjectPtr<UDisplayText> &TextWidget, const FTextColor &Color) {
    if (TextWidget == nullptr)
        return;

    TextWidget->SetTextColor(Color.MainColor);
    TextWidget->SetShadowColor(Color.ShadowColor);
}

void UPokemonUIUtils::SetBarValues(TObjectPtr<UProgressBar> &ProgressBar, float CurrentValue, float MaxValue) {
    if (ProgressBar == nullptr)
        return;

    ProgressBar->SetPercent(FMath::Clamp(CurrentValue / MaxValue, 0, 1));
}

void UPokemonUIUtils::SetPokemonGenderText(EPokemonGender Gender, TObjectPtr<UDisplayText> &TextWidget) {
    using enum EPokemonGender;
    switch (Gender) {
    case Male:
        SetItemText(TextWidget, TEXT("♂"));
        break;
    case Female:
        SetItemText(TextWidget, TEXT("♀"));
        break;
    case Genderless:
        SetItemText(TextWidget, TEXT(""));
        break;
    }
}
