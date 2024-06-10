// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/PokemonBattle.h"
#include "Battle/BattleSide.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Mainpulation/RangeHelpers.h"

static auto GetBattlers(const TScriptInterface<IBattleSide> &Side) {
    return RangeHelpers::CreateRange(Side->GetBattlers());
}

void UPokemonBattle::StartBattle() {
    StartTurn();
}

void UPokemonBattle::QueueAction(const TScriptInterface<IBattleAction> &Action) {
    auto &Battler = Action->GetBattler();
    if (auto BattlerId = Battler->GetInternalId(); ExpectedActionCount.FindChecked(BattlerId) >= CurrentActionCount.FindChecked(BattlerId)) {
        UE_LOG(LogTemp, Error, TEXT("%s attempted to queue an action, but is already at capacity!"),
            *Battler->GetNickname().ToString());
    }
    
    FScopeLock Lock(&ActionMutex);
    ActionQueue.Add(Action);
}

bool UPokemonBattle::ActionSelectionFinished() const {
    for (auto [ID, Count] : ExpectedActionCount) {
        if (CurrentActionCount[ID] < Count) {
            return false;
        }    
    }

    return true;
}

void UPokemonBattle::ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const {
    std::ranges::for_each(RangeHelpers::CreateRange(Sides)
        | std::views::transform(&GetBattlers)
        | std::ranges::views::join, Callback);
}

void UPokemonBattle::StartTurn() {
    TurnCount++;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    ForEachActiveBattler([this](const TScriptInterface<IBattler>& Battler) {
            Battler->SelectActions();
            auto BattlerId = Battler->GetInternalId();
            CurrentActionCount.Add(BattlerId, 0);
            ExpectedActionCount.Add(BattlerId, Battler->ActionCount());
        });
}