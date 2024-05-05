// "Unreal Pokémon" created by Retro & Chill.


#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "Pokemon/Pokemon.h"

void UDefaultAbilityBlock::Initialize(const TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) {
    Owner = NewOwner;
}

FText UDefaultAbilityBlock::GetDisplayName() const {
}

FText UDefaultAbilityBlock::GetAbilityDescription() const {
}

UAbilityEffect * UDefaultAbilityBlock::GetAbilityEffect() const {
    return AbilityEffect;
}

int32 UDefaultAbilityBlock::GetAbilityIndex() const {
    check(Owner != nullptr)
    return static_cast<int32>(Owner->GetPersonalityValue() & 0x1);
}