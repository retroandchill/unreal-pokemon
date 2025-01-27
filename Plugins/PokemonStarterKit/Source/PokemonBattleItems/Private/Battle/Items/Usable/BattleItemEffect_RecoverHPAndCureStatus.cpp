// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/Usable/BattleItemEffect_RecoverHPAndCureStatus.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Battlers/BattlerHelpers.h"
#include "Battle/StatusEffects/BattleStatusEffectUtils.h"
#include "RetroLib/Optionals/OrElseGet.h"

UE5Coro::TCoroutine<bool> UBattleItemEffect_RecoverHPAndCureStatus::ApplyEffectToTarget(
    TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target, FForceLatentCoroutine ForceLatentCoroutine) {
    int32 Recovery = FMath::CeilToInt32(Target->GetAbilityComponent()->GetCoreAttributes()->GetMaxHP() * HPRecovery);
    bool bRemovedStatus = co_await UBattleStatusEffectUtils::RemoveStatusEffectFromBattler(Target);
    co_return bRemovedStatus &&co_await UBattlerHelpers::ApplyHPRecoveryEffect(Target, Recovery, this, !bRemovedStatus);
}