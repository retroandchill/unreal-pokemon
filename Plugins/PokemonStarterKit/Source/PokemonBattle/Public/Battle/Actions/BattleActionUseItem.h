// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMove.h"
#include "BattleActionBase.h"

class IBattleMove;

struct POKEMONBATTLE_API FItemTarget {
    TVariant<TWeakInterfacePtr<IBattler>, TWeakInterfacePtr<IBattleMove>, FTargetWithIndex> Data;

    explicit FItemTarget(TWeakInterfacePtr<IBattler> &&Battler);
    explicit FItemTarget(TWeakInterfacePtr<IBattleMove> &&Move);
    explicit FItemTarget(FTargetWithIndex &&Target);
};

/**
 *
 */
class POKEMONBATTLE_API FBattleActionUseItem : public FBattleActionBase {
  public:
    FBattleActionUseItem(const TScriptInterface<IBattler> &Battler, FName ItemID, FItemTarget &&ItemTarget);
    FString GetReferencerName() const override;
    int32 GetPriority() const override;
    FText GetActionMessage() const override;

  protected:
    UE5Coro::TCoroutine<> ActivateAbility() override;

  private:
    FName ItemID;
    FItemTarget ItemTarget;
};
