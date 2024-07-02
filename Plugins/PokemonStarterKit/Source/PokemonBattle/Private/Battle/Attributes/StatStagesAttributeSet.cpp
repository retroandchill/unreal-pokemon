﻿// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Attributes/StatStagesAttributeSet.h"

void UStatStagesAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    if (Attribute == GetSameTypeAttackBonusAttribute() || Attribute == GetCriticalHitStagesAttribute()) {
        return;
    }
    
    // TODO: Change this to be more customizable
    NewValue = FMath::Floor(FMath::Clamp(NewValue, -6.f, 6.f));
}