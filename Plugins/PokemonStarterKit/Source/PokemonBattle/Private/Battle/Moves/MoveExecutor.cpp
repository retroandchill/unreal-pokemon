// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveExecutor.h"

const TScriptInterface<IBattleMove> &AMoveExecutor::GetMove() const {
    return Move;
}

void AMoveExecutor::SetMove(const TScriptInterface<IBattleMove>& MoveIn) {
    Move = MoveIn;
}

FCompleteCheck & AMoveExecutor::GetCompleteCheckDelegate() {
    return CompleteCheck;
}

void AMoveExecutor::ExecuteCheckComplete(bool bSuccess) const {
    check(CompleteCheck.IsBound())
    CompleteCheck.Execute(bSuccess);
}

void AMoveExecutor::ExecuteExecutorDispatchComplete() const {
    check(OnExecutorDispatchComplete.IsBound())
    OnExecutorDispatchComplete.Execute();
}