// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/Usable/BattleItemEffect_CureStatusEffect.h"
#include "Battle/StatusEffects/BattleStatusEffectUtils.h"

UE5Coro::TCoroutine<bool>
UBattleItemEffect_CureStatusEffect::ApplyEffectToTarget(const TScriptInterface<IBattler> &User,
                                                        const TScriptInterface<IBattler> &Target,
                                                        FForceLatentCoroutine ForceLatentCoroutine) {
    return UBattleStatusEffectUtils::RemoveStatusEffectFromBattler(Target, StatusEffectID);
}