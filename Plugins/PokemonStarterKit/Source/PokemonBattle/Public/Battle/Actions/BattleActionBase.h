// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleAction.h"
#include "UObject/Interface.h"

/**
 * The base action implementation, defines the battler held by the action.
 */
class POKEMONBATTLE_API FBattleActionBase : public FGCObject, public IBattleAction {
public:
    /**
     * Construct a new action with the given battler
     * @param BattlerIn The battler to assign to this action
     */
    explicit FBattleActionBase(const TScriptInterface<IBattler>& BattlerIn);
    
    void AddReferencedObjects(FReferenceCollector &Collector) override;
    const TScriptInterface<IBattler> & GetBattler() const override;

private:
    /**
     * The battler that owns this action
     */
    TScriptInterface<IBattler> Battler;

};