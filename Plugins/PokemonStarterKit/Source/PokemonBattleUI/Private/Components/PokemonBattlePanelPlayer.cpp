// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonBattlePanelPlayer.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/NumberImageWidget.h"

void UPokemonBattlePanelPlayer::Refresh() {
    Super::Refresh();

    auto &Battler = GetCurrentBattler();
    auto CoreAttributes = Battler->GetAbilityComponent()->GetCoreAttributes();
    CurrentHP->SetNumber(static_cast<uint32>(CoreAttributes->GetHP()));
    MaxHP->SetNumber(static_cast<uint32>(CoreAttributes->GetMaxHP()));
    ExpBar->SetPercent(Battler->GetExpPercent());
}

void UPokemonBattlePanelPlayer::UpdateHPPercent(float NewPercent) {
    Super::UpdateHPPercent(NewPercent);
    auto CoreAttributes = GetCurrentBattler()->GetAbilityComponent()->GetCoreAttributes();
    auto CurrentHPValue = static_cast<uint32>(FMath::RoundToFloat(CoreAttributes->GetMaxHP() * NewPercent));
    CurrentHP->SetNumber(CurrentHPValue);
}

void UPokemonBattlePanelPlayer::HPPercentUpdateComplete() const {
    // We need to make sure the correct HP number is displayed
    auto CoreAttributes = GetCurrentBattler()->GetAbilityComponent()->GetCoreAttributes();
    CurrentHP->SetNumber(static_cast<uint32>(CoreAttributes->GetHP()));
    Super::HPPercentUpdateComplete();
}