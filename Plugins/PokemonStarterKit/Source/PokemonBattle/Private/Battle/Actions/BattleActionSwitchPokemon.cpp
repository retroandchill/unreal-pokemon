// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Actions/BattleActionSwitchPokemon.h"
#include "PokemonBattleSettings.h"
#include "Battle/Battlers/Battler.h"

FBattleActionSwitchPokemon::FBattleActionSwitchPokemon(const TScriptInterface<IBattler> &BattlerIn,
    const TScriptInterface<IBattler> &SwitchTargetIn) : FBattleActionBase(BattlerIn), SwitchTarget(SwitchTargetIn) {
}

FString FBattleActionSwitchPokemon::GetReferencerName() const {
    return TEXT("FBattleActionSwitchPokemon");
}

int32 FBattleActionSwitchPokemon::GetPriority() const {
    return GetDefault<UPokemonBattleSettings>()->SwitchPriority;
    
}

FText FBattleActionSwitchPokemon::GetActionMessage() const {
    return GetBattler()->GetRecallMessage();
}

FGameplayAbilitySpecHandle FBattleActionSwitchPokemon::ActivateAbility() {
    return GetBattler()->PerformSwitch(SwitchTarget);
}