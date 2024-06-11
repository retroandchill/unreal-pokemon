// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/PokemonBattle.h"
#include "Battle/BattleSide.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Mainpulation/RangeHelpers.h"

static auto GetBattlers(const TScriptInterface<IBattleSide> &Side) {
    return RangeHelpers::CreateRange(Side->GetBattlers());
}

TScriptInterface<IBattle> UPokemonBattle::Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) {
    Sides = MoveTemp(SidesIn);
    return this;
}

void UPokemonBattle::StartBattle() {
    StartTurn();
}

void UPokemonBattle::QueueAction(TUniquePtr<IBattleAction>&& Action) {
    auto &Battler = Action->GetBattler();
    auto BattlerId = Battler->GetInternalId();
    auto &ActionCount = CurrentActionCount.FindChecked(BattlerId);
    if (ExpectedActionCount.FindChecked(BattlerId) <= ActionCount) {
        UE_LOG(LogTemp, Error, TEXT("%s attempted to queue an action, but is already at capacity!"),
            *Battler->GetNickname().ToString());
        return;
    }
    
    FScopeLock Lock(&ActionMutex);
    ActionQueue.Add(MoveTemp(Action));
    ActionCount++;
}

bool UPokemonBattle::ActionSelectionFinished() const {
    for (auto [ID, Count] : ExpectedActionCount) {
        if (CurrentActionCount[ID] < Count) {
            return false;
        }    
    }

    return true;
}

bool UPokemonBattle::ShouldIgnoreAbilities() const {
    return false;
}

void UPokemonBattle::ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const {
    std::ranges::for_each(RangeHelpers::CreateRange(Sides)
        | std::views::transform(&GetBattlers)
        | std::ranges::views::join, Callback);
}

void UPokemonBattle::ForEachFieldEffect(const TFunctionRef<void(const TScriptInterface<IFieldEffect> &)> Callback) const {
}

bool UPokemonBattle::FindGlobalAbility(FName AbilityID) const {
    return false;
}

void UPokemonBattle::StartTurn() {
    TurnCount++;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    ForEachActiveBattler([this](const TScriptInterface<IBattler>& Battler) {
            auto BattlerId = Battler->GetInternalId();
            CurrentActionCount.Add(BattlerId, 0);
            ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
            Battler->SelectActions();
        });
}