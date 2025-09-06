// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionSwitchPokemon.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Settings/PokemonBattleSettings.h"

FBattleActionSwitchPokemon::FBattleActionSwitchPokemon(const TScriptInterface<IBattler> &BattlerIn,
                                                       const TScriptInterface<IBattler> &SwitchTargetIn)
    : FBattleActionBase(BattlerIn), SwitchTarget(SwitchTargetIn)
{
}

FString FBattleActionSwitchPokemon::GetReferencerName() const
{
    return TEXT("FBattleActionSwitchPokemon");
}

bool FBattleActionSwitchPokemon::CanExecute() const
{
    return GetBattler()->GetOwningSide()->GetBattlers().Contains(GetBattler());
}

int32 FBattleActionSwitchPokemon::GetPriority() const
{
    return GetDefault<UPokemonBattleSettings>()->SwitchPriority;
}

FText FBattleActionSwitchPokemon::GetActionMessage() const
{
    return FText::GetEmpty();
}

UE5Coro::TCoroutine<> FBattleActionSwitchPokemon::ActivateAbility()
{
    return GetBattler()->PerformSwitch(SwitchTarget);
}