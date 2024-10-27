// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Actions/BattleActionSwitchPokemon.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Moves/BattleMove.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Construct.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include <functional>

void UAIBattlerController::InitiateActionSelection(const TScriptInterface<IBattler> &Battler) const {
    // Doing this async is probably overkill, but as the AI gets more complicated and adds more conditions that it
    // needs to evaluate we're going to want to have this be on a separate thread to avoid frame dips while the AI
    // is choosing a move. Ideally we want these threads to resolve quickly (or at least faster than the player can
    // input their commands.
    AsyncTask(ENamedThreads::AnyThread, std::bind_front(&UAIBattlerController::ChooseAction, this, Battler));
}

void UAIBattlerController::InitiateForcedSwitch(const TScriptInterface<IBattler> &Battler) const {
    AsyncTask(ENamedThreads::AnyThread, std::bind_front(&UAIBattlerController::ChoosePokemonToSwitchTo, this, Battler));
}

void UAIBattlerController::BindOnActionReady(FActionReady &&QueueAction) {
    ActionReady = std::move(QueueAction);
}

void UAIBattlerController::ChooseAction(TScriptInterface<IBattler> Battler) const {
    // clang-format off
    auto PossibleMoves = Battler->GetMoves() |
                         UE::Ranges::Filter(&IBattleMove::IsUsable) |
                         UE::Ranges::ToArray;
    // clang-format on

    // TODO: Right now we're just getting a proof of concept for the battle system for now, but eventually we will want
    // this class to call to a series of additional child objects that represent the checks that can be used. It may
    // be best to store a map or a list in a Data Asset that has all of the checks that would be applied and the minimum
    // skill level needed to add those checks. For now though, just choose a random usable move and struggle if there
    // are no such moves.
    auto &Move = PossibleMoves[FMath::Rand() % PossibleMoves.Num()];
    auto Targets = Move->GetAllPossibleTargets() | UE::Ranges::Construct<FTargetWithIndex>() | UE::Ranges::ToArray;
    ActionReady.ExecuteIfBound(MakeUnique<FBattleActionUseMove>(Battler, Move, std::move(Targets)));
}

void UAIBattlerController::ChoosePokemonToSwitchTo(TScriptInterface<IBattler> Battler) const {
    auto &CurrentHandler = Battler->GetWrappedPokemon()->GetCurrentHandler();
    check(CurrentHandler != nullptr)

    auto &HandlerParty = Battler->GetOwningSide()->GetTrainerParty(CurrentHandler);
    auto ViableSwap = HandlerParty.FindByPredicate([](const TScriptInterface<IBattler> &Possibility) {
        return !Possibility->IsActive() && !Possibility->IsFainted();
    });
    check(ViableSwap != nullptr)

    ActionReady.ExecuteIfBound(MakeUnique<FBattleActionSwitchPokemon>(Battler, *ViableSwap));
}