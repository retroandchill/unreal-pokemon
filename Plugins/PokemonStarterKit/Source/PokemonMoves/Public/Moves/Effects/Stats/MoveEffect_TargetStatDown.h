// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Stats/StatChangeHelpers.h"

#include "MoveEffect_TargetStatDown.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class POKEMONMOVES_API UMoveEffect_TargetStatDown : public UBattleMoveFunctionCode {
    GENERATED_BODY()

  public:
    UE5Coro::TCoroutine<bool> FailsAgainstTarget(TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target,
                                                 FForceLatentCoroutine) override;

    UE5Coro::TCoroutine<> ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                TScriptInterface<IBattler> Target) override;

    UE5Coro::TCoroutine<> ApplyEffectAgainstTarget(TScriptInterface<IBattler> User, TScriptInterface<IBattler> Target,
                                                   FForceLatentCoroutine) override;

  private:
    UE5Coro::TCoroutine<> ApplyStatChanges(const TScriptInterface<IBattler> &Target, FForceLatentCoroutine = {});

    UPROPERTY(EditDefaultsOnly, Category = "Battle|Stats")
    TArray<FBattleStatChangeValue> StatsToChange;
};
