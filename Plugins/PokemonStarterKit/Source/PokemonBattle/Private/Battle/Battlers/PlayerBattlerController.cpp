// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Battlers/PlayerBattlerController.h"
#include "Battle/Battle.h"

UPlayerBattlerController *UPlayerBattlerController::SetBattle(const TScriptInterface<IBattle> &Battle) {
    BattleContext = Battle;
    return this;
}

void UPlayerBattlerController::InitiateActionSelection(const TScriptInterface<IBattler> &Battler) const {
    IBattle::Execute_BeginActionSelection(BattleContext.GetObject(), Battler);
}

void UPlayerBattlerController::BindOnActionReady(FActionReady &&QueueAction) {
    ActionReady = MoveTemp(QueueAction);
}