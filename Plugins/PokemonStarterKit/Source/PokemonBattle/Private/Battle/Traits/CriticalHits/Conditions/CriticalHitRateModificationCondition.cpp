// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Traits/CriticalHits/Conditions/CriticalHitRateModificationCondition.h"

bool UCriticalHitRateModificationCondition::Evaluate_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return true;
}