// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonBattlePanel.h"
#include "Battle/Battlers/Battler.h"
#include "Components/ProgressBar.h"
#include "Primatives/DisplayText.h"
#include "Utilities/PokemonUIUtils.h"

void UPokemonBattlePanel::SetBattler(const TScriptInterface<IBattler> &Battler) {
    CurrentBattler = Battler;
    Refresh();
}

void UPokemonBattlePanel::Refresh() {
    check(CurrentBattler != nullptr)
    if (CurrentBattler->IsFainted()) {
        SetVisibility(ESlateVisibility::Hidden);
    } else {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    PokemonName->SetText(CurrentBattler->GetNickname());
    PokemonLevel->SetText(FText::FromString(FString::FromInt(CurrentBattler->GetPokemonLevel())));
    HPBar->SetPercent(CurrentBattler->GetHPPercent());

    auto Gender = CurrentBattler->GetGender();
    UPokemonUIUtils::SetPokemonGenderText(Gender, PokemonGender);
    if (GenderTextColors.Contains(Gender)) {
        UPokemonUIUtils::SetItemTextColor(PokemonGender, GenderTextColors[Gender]);
    }
}

const TScriptInterface<IBattler> &UPokemonBattlePanel::GetCurrentBattler() const {
    return CurrentBattler;
}