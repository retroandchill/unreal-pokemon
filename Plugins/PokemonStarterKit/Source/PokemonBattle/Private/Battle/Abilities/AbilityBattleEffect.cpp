// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Abilities/AbilityBattleEffect.h"

const FIndividualDamageModifierTraits &UAbilityBattleEffect::GetDamageModifiers() const {
    return DamageModifiers;
}

bool UAbilityBattleEffect::HasTag(FName Tag) const {
    return Tags.Contains(Tag);
}