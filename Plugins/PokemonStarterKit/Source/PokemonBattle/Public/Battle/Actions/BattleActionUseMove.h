// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleActionBase.h"

class IBattleMove;
/**
 * The battle action for using a move
 */
class POKEMONBATTLE_API FBattleActionUseMove : public FBattleActionBase {
public:
    /**
     * Construct a new action with the given battler using the selected move
     * @param BattlerIn The battler for the action
     * @param MoveIn The move to be used
     */
    FBattleActionUseMove(const TScriptInterface<IBattler> &BattlerIn, const TScriptInterface<IBattleMove>& MoveIn);

    void AddReferencedObjects(FReferenceCollector &Collector) override;
    FString GetReferencerName() const override;

private:
    /**
     * The move that this action owns
     */
    TScriptInterface<IBattleMove> Move;
};
