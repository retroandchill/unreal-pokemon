// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Moves/PromptReplaceMove.h"
#include "Moves/MoveData.h"
#include "Utilities/PokemonUIAsyncActions.h"

UPromptReplaceMove *UPromptReplaceMove::PromptReplaceMove(const UObject *WorldContextObject,
                                                          const TScriptInterface<IPokemon> &Pokemon,
                                                          const FMoveHandle &Move) {
    auto Node = NewObject<UPromptReplaceMove>();
    Node->SetWorldContext(WorldContextObject);
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

UE5Coro::TCoroutine<> UPromptReplaceMove::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    if (co_await Pokemon::UI::PromptReplaceMove(GetWorldContext(), Pokemon, Move)) {
        MoveLearned.Broadcast();
    } else {
        MoveNotLearned.Broadcast();
    }
}