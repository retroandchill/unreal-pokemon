// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonBattlePanelPlayer.h"
#include "Battle/Battlers/Battler.h"
#include "Components/ProgressBar.h"
#include "Primatives/NumberImageWidget.h"

void UPokemonBattlePanelPlayer::Refresh() {
    Super::Refresh();

    auto &Battler = GetCurrentBattler();
    CurrentHP->SetNumber(static_cast<uint32>(Battler->GetHP()));
    MaxHP->SetNumber(static_cast<uint32>(Battler->GetMaxHP()));
    ExpBar->SetPercent(Battler->GetExpPercent());
}