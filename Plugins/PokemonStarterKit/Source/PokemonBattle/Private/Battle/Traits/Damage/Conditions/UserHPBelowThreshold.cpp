// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/Damage/Conditions/UserHPBelowThreshold.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"

bool UUserHPBelowThreshold::Evaluate_Implementation(const FMoveDamageInfo &Context) const {
    return Context.User->GetHPPercent() <= Threshold;
}