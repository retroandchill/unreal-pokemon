// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Traits/CriticalHits/CriticalHitOverrideTrait.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"

void UCriticalHitOverrideTrait::Apply_Implementation(int32 &Stages, ECriticalOverride &Override,
                                                     const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const {
    Override = UMoveEvaluationHelpers::ApplyCriticalHitOverride(Override, OverrideValue);
}