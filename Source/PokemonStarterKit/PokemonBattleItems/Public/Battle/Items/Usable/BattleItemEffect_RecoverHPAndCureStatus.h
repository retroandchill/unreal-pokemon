// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Items/BattleItemEffect.h"

#include "BattleItemEffect_RecoverHPAndCureStatus.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, DisplayName = "Battle Item Effect Recover HP and Cure Status")
class POKEMONBATTLEITEMS_API UBattleItemEffect_RecoverHPAndCureStatus : public UBattleItemEffect
{
    GENERATED_BODY()

  protected:
    UE5Coro::TCoroutine<bool> ApplyEffectToTarget(TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target,
                                                  FForceLatentCoroutine) override;

  private:
    UPROPERTY(EditDefaultsOnly, DisplayName = "HP Recovery", Category = "Battle|Items",
              meta = (ClampMin = 0, UIMin = 0, ClampMax = 1, UIMax = 1))
    float HPRecovery = 1.f;
};
