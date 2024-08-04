// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleAction.h"

/**
 * Action defined where the character does nothing. It is used to skip the battler's action entirely
 */
class POKEMONBATTLE_API FBattleActionDoNothing : public FGCObject, public IBattleAction {
  public:
    /**
     * Construct a new action with the given battler
     * @param BattlerIn The battler to assign to this action
     */
    explicit FBattleActionDoNothing(const TScriptInterface<IBattler> &BattlerIn);

    void AddReferencedObjects(FReferenceCollector &Collector) override;
    FString GetReferencerName() const override;
    const TScriptInterface<IBattler> &GetBattler() const override;
    bool CanExecute() const override;
    void Execute() override;
    bool IsExecuting() const override;
    bool IsComplete() const override;
    int32 GetPriority() const override;
    FText GetActionMessage() const override;

  private:
    TScriptInterface<IBattler> Battler;
};
