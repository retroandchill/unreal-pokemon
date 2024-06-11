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
    FBattleActionUseMove(const TScriptInterface<IBattler> &BattlerIn, const TScriptInterface<IBattleMove>& MoveIn);

    void AddReferencedObjects(FReferenceCollector &Collector) override;
    FString GetReferencerName() const override;

private:
    TScriptInterface<IBattleMove> Move;
};
