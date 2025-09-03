// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Items/BattleItemEffect.h"
#include "Battle/Status.h"

#include "BattleItemEffect_CureStatusEffect.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class POKEMONBATTLEITEMS_API UBattleItemEffect_CureStatusEffect : public UBattleItemEffect {
    GENERATED_BODY()

  protected:
    UE5Coro::TCoroutine<bool> ApplyEffectToTarget(TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target,
                                                  FForceLatentCoroutine) override;

  private:
    UPROPERTY(EditDefaultsOnly, Category = "Battle|Items")
    FStatusHandle StatusEffectID;
};
