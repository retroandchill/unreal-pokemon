// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/LearnMove.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Utilities/PokemonUtilities.h"

ULearnMove *ULearnMove::LearnMove(const TScriptInterface<IPokemon> &Pokemon, FName Move) {
    auto Node = NewObject<ULearnMove>();
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

void ULearnMove::Activate() {
    FMoveLearnEnd MoveLearnEnd;
    MoveLearnEnd.BindDynamic(this, &ULearnMove::ExecuteMoveLearnedOrRejected);
    Pokemon->GetMoveBlock()->LearnMove(Move, MoveLearnEnd);
}

void ULearnMove::ExecuteMoveLearnedOrRejected(bool bMoveLearned) {
    if (bMoveLearned) {
        MoveLearned.Broadcast();
    } else {
        MoveRejected.Broadcast();
    }
    SetReadyToDestroy();
}