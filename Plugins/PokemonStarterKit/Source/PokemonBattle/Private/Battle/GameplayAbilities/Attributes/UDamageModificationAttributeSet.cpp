// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Attributes/DamageModificationAttributeSet.h"

void UDamageModificationAttributeSet::Reset() {
    InitPowerMultiplier(1.f);
    InitAttackMultiplier(1.f);
    InitDefenseMultiplier(1.f);
    InitFinalDamageMultiplier(1.f);
    InitCriticalHitMultiplier(Pokemon::FBaseSettings::Get().GetCriticalHitDamageMod());
}
