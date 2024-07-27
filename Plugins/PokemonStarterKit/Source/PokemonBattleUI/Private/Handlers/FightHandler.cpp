// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/FightHandler.h"
#include "Screens/PokemonBattleScreen.h"
#include "Components/PokemonActionOptions.h"

void UFightHandler::OnSelected_Implementation(UPokemonBattleScreen *BattleScreen) {
    auto ActionSelect = BattleScreen->GetActionSelect();
    BattleScreen->SelectMove(ActionSelect->GetCurrentBattler());
}