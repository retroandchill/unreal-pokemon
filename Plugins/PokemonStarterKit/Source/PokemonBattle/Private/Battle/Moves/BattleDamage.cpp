// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BattleDamage.h"
#include "Battle/GameplayAbilities/Attributes/DamageModificationAttributeSet.h"

FDamageMultiplierHandler::FDamageMultiplierHandler() = default;

FDamageMultiplierHandler::FDamageMultiplierHandler(const UDamageModificationAttributeSet &Attributes) {
    Multipliers->PowerMultiplier = Attributes.GetPowerMultiplier();
    Multipliers->AttackMultiplier = Attributes.GetAttackMultiplier();
    Multipliers->DefenseMultiplier = Attributes.GetDefenseMultiplier();
    Multipliers->FinalDamageMultiplier = Attributes.GetFinalDamageMultiplier();
}

FMoveDamageInfo::FMoveDamageInfo() = default;

FMoveDamageInfo::FMoveDamageInfo(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                 int32 TargetCount) : User(User), Target(Target), TargetCount(TargetCount) {
}