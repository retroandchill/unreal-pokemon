// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonCoreSettings.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

FName UPokemonCoreSettings::GetHPStat() const {
    return HPStat;
}

FText UPokemonCoreSettings::GetNoAbilityText() const {
    return NoAbilityText;
}

FText UPokemonCoreSettings::GetNoAbilityDescription() const {
    return NoAbilityDescription;
}

int32 UPokemonCoreSettings::GetMaxPartySize() const {
    return MaxPartySize;
}

int32 UPokemonCoreSettings::GetMaxMoves() const {
    return MaxMoves;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetPokemonClass() const {
    return PokemonClass;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetStatBlockClass() const {
    return StatBlockClass;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetMoveBlockClass() const {
    return MoveBlockClass;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetAbilityBlockClass() const {
    return AbilityBlockClass;
}

uint32 UPokemonCoreSettings::GetShinyChance() const {
    return ShinyChance;
}
