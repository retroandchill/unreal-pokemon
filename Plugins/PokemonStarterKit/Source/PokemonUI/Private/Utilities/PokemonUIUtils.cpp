﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonUIUtils.h"
#include "Components/DisplayText.h"
#include "Components/ProgressBar.h"
#include "Pokemon/Pokemon.h"

enum class EPokemonGender : uint8;

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

void UPokemonUIUtils::SetBarValues(TObjectPtr<UProgressBar> &ProgressBar, float CurrentValue, float MaxValue) {
    if (ProgressBar == nullptr)
        return;

    ProgressBar->SetPercent(FMath::Clamp(CurrentValue / MaxValue, 0, 1));
}

void UPokemonUIUtils::SetPokemonGenderText(EPokemonGender Gender, UDisplayText *TextWidget) {
    using enum EPokemonGender;
    switch (Gender) {
    case Male:
        TextWidget->SetText(FText::FromStringView(TEXT("♂")));
        break;
    case Female:
        TextWidget->SetText(FText::FromStringView(TEXT("♀")));
        break;
    case Genderless:
        TextWidget->SetText(FText::FromStringView(TEXT("")));
        break;
    }
}
