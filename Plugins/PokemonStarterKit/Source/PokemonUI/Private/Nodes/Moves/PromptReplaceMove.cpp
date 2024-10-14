// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Moves/PromptReplaceMove.h"
#include "Moves/MoveData.h"
#include "Screens/MoveForgetScreen.h"

UPromptReplaceMove *UPromptReplaceMove::PromptReplaceMove(const UObject *WorldContextObject,
                                                          const TScriptInterface<IPokemon> &Pokemon,
                                                          const FMoveHandle &Move) {
    auto Node = NewObject<UPromptReplaceMove>();
    Node->WorldContextObject = WorldContextObject;
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

void UPromptReplaceMove::Activate() {

    auto Screen = UMoveForgetScreen::AddMoveForgetScreenToStack(WorldContextObject);
    Screen->InitializeScene(Pokemon, Move);
    Screen->BindToOnMoveForgetComplete(this, &UPromptReplaceMove::OnMoveSelectionComplete);
}

void UPromptReplaceMove::OnMoveSelectionComplete(bool bMoveReplaced) {
    if (bMoveReplaced) {
        MoveLearned.Broadcast();
    } else {
        MoveNotLearned.Broadcast();
    }
    SetReadyToDestroy();
}