// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMove.h"
#include "UObject/Object.h"

#include "TestBattleMove.generated.h"

/**
 *
 */
UCLASS(BlueprintInternalUseOnly)
class POKEMONTESTS_API UTestBattleMove : public UObject, public IBattleMove {
    GENERATED_BODY()

  public:
    int32 CostsPaid = 0;

    TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IBattle> &Battle,
                                             const TScriptInterface<IMove> &Move) override;
    bool IsConfusionAttack() const override;
    bool HasHighCriticalHitRate() const override;
    bool HasTag(FName Tag) const override;

  protected:
    FText GetDisplayName_Implementation() const override;
    int32 GetPriority_Implementation() const override;
    void PayCost_Implementation(int32 Amount) override;
    bool PerformHitCheck_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) override;
    FBattleDamage CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                 const TScriptInterface<IBattler> &Target, int32 TargetCount) override;
};

inline int32 UTestBattleMove::GetPriority_Implementation() const {
    return 1;
}
