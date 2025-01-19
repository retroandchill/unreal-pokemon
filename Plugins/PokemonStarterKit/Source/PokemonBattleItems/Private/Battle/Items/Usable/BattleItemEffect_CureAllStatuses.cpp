﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/Usable/BattleItemEffect_CureAllStatuses.h"
#include "Battle/StatusEffects/BattleStatusEffectUtils.h"

UE5Coro::TCoroutine<bool>
UBattleItemEffect_CureAllStatuses::ApplyEffectToTarget(TScriptInterface<IBattler> User,
                                                       TScriptInterface<IBattler> Target,
                                                       FForceLatentCoroutine ForceLatentCoroutine) {
    return UBattleStatusEffectUtils::RemoveStatusEffectFromBattler(Target);
}