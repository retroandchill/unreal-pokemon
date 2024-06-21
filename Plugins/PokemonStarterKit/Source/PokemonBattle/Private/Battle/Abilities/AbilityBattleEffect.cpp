// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Abilities/AbilityBattleEffect.h"

const FIndividualDamageModifierTraits &UAbilityBattleEffect::GetDamageModifiers() const {
    return DamageModifiers;
}

const FIndividualCriticalHitRateModifierTraits & UAbilityBattleEffect::GetCriticalHitRateModifiers() const {
    return CriticalHitRateModifiers;
}

bool UAbilityBattleEffect::HasTag(FName Tag) const {
    return Tags.Contains(Tag);
}