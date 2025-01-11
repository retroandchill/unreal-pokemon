// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/PlayerBattlerController.h"
#include "Battle/Battle.h"

UPlayerBattlerController *UPlayerBattlerController::SetBattle(const TScriptInterface<IBattle> &Battle) {
    BattleContext = Battle;
    return this;
}

void UPlayerBattlerController::ActionSelection(const TScriptInterface<IBattler> &Battler) const {
    IBattle::Execute_BeginActionSelection(BattleContext.GetObject(), Battler);
}

void UPlayerBattlerController::InitiateForcedSwitch(const TScriptInterface<IBattler> &Battler) const {
    IBattle::Execute_PromptMandatorySwitch(BattleContext.GetObject(), Battler);
}

void UPlayerBattlerController::BindOnActionReady(FActionReady &&QueueAction) {
    ActionReady = std::move(QueueAction);
}