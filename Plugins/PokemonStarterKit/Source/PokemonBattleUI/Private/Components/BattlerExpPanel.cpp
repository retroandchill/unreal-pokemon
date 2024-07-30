// "Unreal Pokémon" created by Retro & Chill.


#include "Components/BattlerExpPanel.h"
#include "Battle/Battlers/Battler.h"
#include "Components/DisplayText.h"

void UBattlerExpPanel::SetBattler(const TScriptInterface<IBattler> &Battler) {
    CurrentBattler = Battler;
    LevelText->SetText(FText::FromString(FString::FromInt(Battler->GetPokemonLevel())));
    ChangeExpGainDisplay(0);
    OnBattlerSet(CurrentBattler);
}

void UBattlerExpPanel::ChangeExpGainDisplay(int32 Gain) {
    static const auto GainFormat = FText::FromStringView(TEXT("+{0}"));
    ExpGainText->SetText(FText::Format(GainFormat, { Gain }));
}