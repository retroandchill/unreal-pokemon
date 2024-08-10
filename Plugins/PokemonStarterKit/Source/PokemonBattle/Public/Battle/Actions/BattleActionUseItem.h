// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleActionBase.h"
#include "Battle/Moves/BattleMove.h"

class IBattleMove;

struct POKEMONBATTLE_API FItemTarget {
    TVariant<TWeakInterfacePtr<IBattler>, TWeakInterfacePtr<IBattleMove>, FTargetWithIndex> Data;

    explicit FItemTarget(TWeakInterfacePtr<IBattler>&& Battler);
    explicit FItemTarget(TWeakInterfacePtr<IBattleMove>&& Move);
    explicit FItemTarget(FTargetWithIndex&& Target);
};



/**
 * 
 */
class POKEMONBATTLE_API FBattleActionUseItem : public FBattleActionBase {
public:
    FBattleActionUseItem(const TScriptInterface<IBattler>& Battler, FName ItemID, FItemTarget&& ItemTarget);
    virtual FString GetReferencerName() const override;
    virtual int32 GetPriority() const override;
    virtual FText GetActionMessage() const override;

protected:
    virtual FGameplayAbilitySpecHandle ActivateAbility() override;

private:
    FName ItemID;
    FItemTarget ItemTarget;
};
