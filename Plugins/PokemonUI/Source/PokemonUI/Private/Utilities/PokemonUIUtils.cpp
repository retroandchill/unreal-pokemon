﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonUIUtils.h"

#include "Components/ProgressBar.h"
#include "Primatives/DisplayText.h"
#include "Text/ShadowedText.h"
#include "Text/TextColor.h"

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

    if (auto ShadowedText = Cast<UShadowedText>(TextWidget); ShadowedText != nullptr) {
        ShadowedText->SetShadowColor(Color.ShadowColor);
    }
}

void UPokemonUIUtils::SetBarValues(TObjectPtr<UProgressBar> &ProgressBar, float CurrentValue, float MaxValue) {
    if (ProgressBar == nullptr)
        return;

    ProgressBar->SetPercent(FMath::Clamp(CurrentValue / MaxValue, 0, 1));
}
