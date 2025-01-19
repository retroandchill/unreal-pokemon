// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Items/BattleItemEffect.h"
#include "BattleItemEffect_CureAllStatuses.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class POKEMONBATTLEITEMS_API UBattleItemEffect_CureAllStatuses : public UBattleItemEffect {
    GENERATED_BODY()

protected:
    UE5Coro::TCoroutine<bool> ApplyEffectToTarget(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, FForceLatentCoroutine) override;

};
