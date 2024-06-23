// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BattleDamage.h"

FMoveDamageInfo::FMoveDamageInfo() = default;

FMoveDamageInfo::FMoveDamageInfo(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                 int32 TargetCount) : User(User), Target(Target), TargetCount(TargetCount) {
}