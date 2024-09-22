// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/LearnMove.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Moves/MoveBlock.h"

ULearnMove *ULearnMove::LearnMove(const TScriptInterface<IPokemon> &Pokemon, FMoveHandle Move) {
    auto Node = NewObject<ULearnMove>();
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

void ULearnMove::Activate() {
    Pokemon->GetMoveBlock()->LearnMove(Move,
        FOnMoveLearnEnd::FDelegate::CreateUObject(this, &ULearnMove::ExecuteMoveLearnedOrRejected));
}

void ULearnMove::ExecuteMoveLearnedOrRejected(bool bMoveLearned) {
    if (bMoveLearned) {
        MoveLearned.Broadcast();
    } else {
        MoveRejected.Broadcast();
    }
    SetReadyToDestroy();
}