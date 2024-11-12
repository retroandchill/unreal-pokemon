// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonUIUtils.h"
#include "Components/ProgressBar.h"
#include "Pokemon/Pokemon.h"
#include "Ranges/Utilities/Unreachable.h"

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

FText UPokemonUIUtils::GetPokemonGenderText(EPokemonGender Gender) {
    using enum EPokemonGender;
    switch (Gender) {
    case Male:
        return FText::FromStringView(TEXT("♂"));
    case Female:
        return FText::FromStringView(TEXT("♀"));
    case Genderless:
        return FText::FromStringView(TEXT(""));
    }

    check(false)
    UE::Ranges::Unreachable();
}
