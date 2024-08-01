// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleActionBase.h"

/**
 * Battle action for switching Pokémon
 */
class POKEMONBATTLE_API FBattleActionSwitchPokemon : public FBattleActionBase {
public:
    /**
     * Construct a new action with the given battler
     * @param BattlerIn The battler to assign to this action
     * @oparam SwitchTargetIn The Pokémon to switch to
     */
    explicit FBattleActionSwitchPokemon(const TScriptInterface<IBattler> &BattlerIn, const TScriptInterface<IBattler>& SwitchTargetIn);
    FString GetReferencerName() const override;
    int32 GetPriority() const override;
    FText GetActionMessage() const override;

protected:
    FGameplayAbilitySpecHandle ActivateAbility() override;

private:
    TScriptInterface<IBattler> SwitchTarget;
};
