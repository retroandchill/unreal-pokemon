// "Unreal Pokémon" created by Retro & Chill.


#include "TestCritRateModCondition.h"

bool UTestCritRateModCondition::Evaluate_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return bState;
}