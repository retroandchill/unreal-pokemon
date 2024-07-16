// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionHelpers.h"
#include "Battle/Transitions/WildBattleOpponentInfo.h"

FBattleInfo UBattleTransitionHelpers::MakeSingleWildBattle(FName Species, int32 Level, bool bLossAllowed) {
    auto PokemonDTO = MakeShared<FPokemonDTO>(FPokemonDTO{.Species = Species, .Level = Level});
    return {.bLossAllowed = bLossAllowed, .OpponentInfo = {.Data = MakeShared<FWildBattleOpponentInfo>(PokemonDTO)}};
}