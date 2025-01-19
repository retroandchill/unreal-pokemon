// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Stats/StatChangeHelpers.h"

#include "MoveEffect_StatUp.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class POKEMONMOVES_API UMoveEffect_StatUp : public UBattleMoveFunctionCode {
    GENERATED_BODY()

  protected:
    UE5Coro::TCoroutine<bool> MoveFailed(TScriptInterface<IBattler> User,
                                         const TArray<TScriptInterface<IBattler>> &Targets,
                                         FForceLatentCoroutine) override;

    UE5Coro::TCoroutine<> ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                TScriptInterface<IBattler> Target) override;

    UE5Coro::TCoroutine<> ApplyGeneralEffect(TScriptInterface<IBattler> User, FForceLatentCoroutine) override;

  private:
    UE5Coro::TCoroutine<> ApplyStatChanges(const TScriptInterface<IBattler> &User, FForceLatentCoroutine = {});

    UPROPERTY(EditDefaultsOnly, Category = "Battle|Stats")
    TArray<FBattleStatChangeValue> StatsToChange;
};
