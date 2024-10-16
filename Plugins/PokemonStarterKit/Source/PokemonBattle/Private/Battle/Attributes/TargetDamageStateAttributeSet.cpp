﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Attributes/TargetDamageStateAttributeSet.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/Type.h"

void UTargetDamageStateAttributeSet::Reset() {
    SetTypeMod(Pokemon::TypeEffectiveness::IneffectiveMultiplier);
    SetCalculatedDamage(0.f);
    GetOwningAbilitySystemComponent()->RemoveLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget);
}