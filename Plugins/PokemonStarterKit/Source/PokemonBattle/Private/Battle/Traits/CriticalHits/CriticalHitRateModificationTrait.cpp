// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTrait.h"
#include "Algo/AllOf.h"
#include "Battle/Traits/CriticalHits/Conditions/CriticalHitRateModificationCondition.h"

#include <functional>

bool EvaluateCondition(const TScriptInterface<IBattler> &User,
                       const TScriptInterface<IBattler> &Target, const UCriticalHitRateModificationCondition *Condition) {
    return Condition->Evaluate(User, Target);
}

bool UCriticalHitRateModificationTrait::MeetsConditions(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return Algo::AllOf(Conditions, std::bind_front(&EvaluateCondition, User, Target));
}

void UCriticalHitRateModificationTrait::Apply_Implementation(int32 &Stages,
                                                             ECriticalOverride& Override,
                                                             const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const {
    // Do nothing
}