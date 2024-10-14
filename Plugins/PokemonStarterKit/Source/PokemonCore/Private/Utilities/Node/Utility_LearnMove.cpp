// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/Node/Utility_LearnMove.h"

DEFINE_INJECTABLE_DEPENDENCY(UUtility_LearnMove)

void UUtility_LearnMove::Execute(const TScriptInterface<IPokemon> &Pokemon, const FMoveHandle &Move,
                                 FOnMoveLearnEnd::FDelegate &&OnEnd) {
    EndProcess.Add(MoveTemp(OnEnd));
    Execute(Pokemon, Move);
}

void UUtility_LearnMove::ExecuteOnEnd(bool bMoveLearned) {
    EndProcess.Broadcast(bMoveLearned);
    Destruct();
}