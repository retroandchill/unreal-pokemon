// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/Actions/ActionResult.h"

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

void UMoveEvaluationHelpers::AlterStatStages(const TScriptInterface<IBattler> &Battler, FAdditionalMoveEffects& Effects, const FBattleStat& Stat, int32 Stages) {
    Effects.AlterStatStage(*Battler, Stat.Stat, Stages);
}