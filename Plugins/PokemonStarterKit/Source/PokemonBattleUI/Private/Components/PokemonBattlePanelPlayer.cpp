// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonBattlePanelPlayer.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Primatives/NumberImageWidget.h"

void UPokemonBattlePanelPlayer::Refresh() {
    Super::Refresh();

    auto &Battler = GetCurrentBattler();
    auto CoreAttributes = Battler->GetAbilityComponent()->GetCoreAttributes();
    CurrentHP->SetNumber(static_cast<uint32>(CoreAttributes->GetHP()));
    MaxHP->SetNumber(static_cast<uint32>(CoreAttributes->GetMaxHP()));
    ExpBar->SetPercent(Battler->GetExpPercent());
}