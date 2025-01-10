// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/LearnMove.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"

ULearnMove *ULearnMove::LearnMove(const TScriptInterface<IPokemon> &Pokemon, FMoveHandle Move) {
    auto Node = NewObject<ULearnMove>();
    Node->SetWorldContext(Pokemon.GetObject());
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

UE5Coro::TCoroutine<> ULearnMove::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    if (co_await Pokemon->GetMoveBlock()->LearnMove(Move)) {
        MoveLearned.Broadcast();
    } else {
        MoveRejected.Broadcast();
    }
}