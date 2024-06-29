// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Attributes/TargetDamageStateAttributeSet.h"
#include "Battle/Type.h"

void UTargetDamageStateAttributeSet::Reset() {
    TypeMod = Pokemon::TypeEffectiveness::IneffectiveMultiplier;
    CalculatedDamage = 0.f;
}