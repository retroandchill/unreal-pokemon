﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Items/BattleItemEffect.h"

#include "BattleItemEffect_HPRecoveryPercent.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, DisplayName = "Battle Item Effect HP Recovery Percent")
class POKEMONBATTLEITEMS_API UBattleItemEffect_HPRecoveryPercent : public UBattleItemEffect {
    GENERATED_BODY()

  protected:
    UE5Coro::TCoroutine<bool> ApplyEffectToTarget(TScriptInterface<IBattler> User,
                                                  TScriptInterface<IBattler> Target,
                                                  FForceLatentCoroutine) override;

  private:
    UPROPERTY(EditDefaultsOnly, DisplayName = "HP Recovery", Category = "Battle|Items",
              meta = (ClampMin = 0, UIMin = 0, ClampMax = 1, UIMax = 1))
    float HPRecovery = 1.f;
};
