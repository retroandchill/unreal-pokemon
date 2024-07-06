// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Transitions/BattleTransitionSubsystem.h"

void UBattleTransitionSubsystem::SetRegisteredBattle(const TScriptInterface<IBattle> &Battle) {
    RegisteredBattle = Battle;
}

void UBattleTransitionSubsystem::InitiateWildBattle(const FPokemonDTO &Pokemon, const TSoftObjectPtr<UWorld>& BattleMap) {
    
}