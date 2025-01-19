// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/Usable/BattleItemEffect_HPRecovery.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Battlers/BattlerHelpers.h"
#include "RetroLib/Optionals/OrElseGet.h"

UE5Coro::TCoroutine<bool>
UBattleItemEffect_HPRecovery::ApplyEffectToTarget(TScriptInterface<IBattler> User,
                                                  TScriptInterface<IBattler> Target,
                                                  FForceLatentCoroutine ForceLatentCoroutine) {
    return UBattlerHelpers::ApplyHPRecoveryEffect(Target, HPRecovery, this);
}