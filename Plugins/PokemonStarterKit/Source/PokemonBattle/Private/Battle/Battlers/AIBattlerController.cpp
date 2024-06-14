// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleMove.h"
#include "Mainpulation/RangeHelpers.h"
#include <functional>

static bool IsMoveUsable(const TScriptInterface<IBattleMove> &Move) {
    return IBattleMove::Execute_IsUsable(Move.GetObject());
}

void UAIBattlerController::InitiateActionSelection(const TScriptInterface<IBattler> &Battler) const {
    // Doing this async is probably overkill, but as the AI gets more complicated and adds more conditions that it
    // needs to evaluate we're going to want to have this be on a separate thread to avoid frame dips while the AI
    // is choosing a move. Ideally we want these threads to resolve quickly (or at least faster than the player can
    // input their commands.
    AsyncTask(ENamedThreads::AnyThread, std::bind_front(&UAIBattlerController::ChooseAction, this, Battler));
}

void UAIBattlerController::BindOnActionReady(FActionReady &&QueueAction) {
    ActionReady = MoveTemp(QueueAction);
}

void UAIBattlerController::ChooseAction(TScriptInterface<IBattler> Battler) const {
    auto PossibleMoves = RangeHelpers::CreateRange(Battler->GetMoves()) | std::views::filter(&IsMoveUsable) |
                         RangeHelpers::TToArray<TScriptInterface<IBattleMove>>();

    // TODO: Right now we're just getting a proof of concept for the battle system for now, but eventually we will want
    // this class to call to a series of additional child objects that represent the checks that can be used. It may
    // be best to store a map or a list in a Data Asset that has all of the checks that would be applied and the minimum
    // skill level needed to add those checks. For now though, just choose a random usable move and struggle if there
    // are no such moves.
    auto &Move = PossibleMoves[FMath::Rand() % PossibleMoves.Num()];
    auto Targets = IBattleMove::Execute_GetAllPossibleTargets(Move.GetObject(), Battler);
    ActionReady.ExecuteIfBound(MakeUnique<FBattleActionUseMove>(Battler, Move, MoveTemp(Targets)));
}