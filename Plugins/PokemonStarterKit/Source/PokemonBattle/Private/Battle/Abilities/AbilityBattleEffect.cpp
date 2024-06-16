// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Abilities/AbilityBattleEffect.h"

const FAbilityDamageModifiers & UAbilityBattleEffect::GetDamageModifiers() const {
    return DamageModifiers;
}