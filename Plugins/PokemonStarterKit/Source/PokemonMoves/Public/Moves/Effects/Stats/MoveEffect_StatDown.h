// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Stats/StatChangeHelpers.h"

#include "MoveEffect_StatDown.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class POKEMONMOVES_API UMoveEffect_StatDown : public UBattleMoveFunctionCode {
    GENERATED_BODY()

  public:
    UE5Coro::TCoroutine<> ApplyEffectWhenDealingDamage(const TScriptInterface<IBattler> &User,
                                                       const TScriptInterface<IBattler> &Target,
                                                       FForceLatentCoroutine) override;

  private:
    UPROPERTY(EditDefaultsOnly, Category = "Battle|Stats")
    TArray<FBattleStatChangeValue> StatsToChange;
};
