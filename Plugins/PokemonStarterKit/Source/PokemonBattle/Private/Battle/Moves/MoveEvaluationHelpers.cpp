// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/GameplayAbilities/Attributes/CriticalHitRateModificationAttributeSet.h"

ECriticalOverride UMoveEvaluationHelpers::ApplyCriticalHitOverride(ECriticalOverride Old, ECriticalOverride New) {
    using enum ECriticalOverride;
    if (Old == Normal) {
        return New;
    }

    if (Old == Always && New == Never) {
        return New;
    }

    // If the old value is set to Never, then we go with that, no questions asked
    return Old;
}

float UMoveEvaluationHelpers::OverrideToAttributeValue(ECriticalOverride Override) {
    switch (Override) {
    case ECriticalOverride::Always:
        return CriticalHits::Always;
    case ECriticalOverride::Never:
        return CriticalHits::Never;
    default:
        return CriticalHits::Normal;        
    }
}

ECriticalOverride UMoveEvaluationHelpers::AttributeValueToOverride(float Value) {
    if (Value >= CriticalHits::Always) {
        return ECriticalOverride::Always;
    }

    if (Value <= CriticalHits::Never) {
        return ECriticalOverride::Never;
    }

    return ECriticalOverride::Normal;
}

FDamageMultipliers & UMoveEvaluationHelpers::GetDamageMultipliers(const FDamageMultiplierHandler &Handler) {
    return *Handler.Multipliers;
}