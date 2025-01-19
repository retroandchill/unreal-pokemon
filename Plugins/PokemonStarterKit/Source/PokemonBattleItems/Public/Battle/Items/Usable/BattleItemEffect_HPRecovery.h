// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Items/BattleItemEffect.h"

#include "BattleItemEffect_HPRecovery.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, DisplayName = "Battle Item Effect HP Recovery")
class POKEMONBATTLEITEMS_API UBattleItemEffect_HPRecovery : public UBattleItemEffect {
    GENERATED_BODY()

  protected:
    UE5Coro::TCoroutine<bool> ApplyEffectToTarget(TScriptInterface<IBattler> User,
                                                  TScriptInterface<IBattler> Target,
                                                  FForceLatentCoroutine) override;

  private:
    UPROPERTY(EditDefaultsOnly, DisplayName = "HP Recovery", Category = "Battle|Items",
              meta = (ClampMin = 1, UIMin = 1))
    int32 HPRecovery;
};
