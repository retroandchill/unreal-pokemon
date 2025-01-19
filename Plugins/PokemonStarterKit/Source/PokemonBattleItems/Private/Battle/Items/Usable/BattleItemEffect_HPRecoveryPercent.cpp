// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/Usable/BattleItemEffect_HPRecoveryPercent.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Battlers/BattlerHelpers.h"

UE5Coro::TCoroutine<bool>
UBattleItemEffect_HPRecoveryPercent::ApplyEffectToTarget(TScriptInterface<IBattler> User,
                                                         TScriptInterface<IBattler> Target,
                                                         FForceLatentCoroutine ForceLatentCoroutine) {
    int32 Recovery = FMath::CeilToInt32(Target->GetAbilityComponent()->GetCoreAttributes()->GetMaxHP() * HPRecovery);
    return UBattlerHelpers::ApplyHPRecoveryEffect(Target, Recovery, this);
}