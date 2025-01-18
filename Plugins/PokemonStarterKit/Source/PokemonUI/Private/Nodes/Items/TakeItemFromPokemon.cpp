// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/TakeItemFromPokemon.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UTakeItemFromPokemon *UTakeItemFromPokemon::TakeItemFromPokemon(const TScriptInterface<IPokemon> &Pokemon) {
    auto Node = NewObject<UTakeItemFromPokemon>();
    Node->SetWorldContext(Pokemon.GetObject());
    Node->Pokemon = Pokemon;
    return Node;
}

UE5Coro::TCoroutine<> UTakeItemFromPokemon::ExecuteCoroutine(FForceLatentCoroutine) {
    if (co_await IPokemonCoroutineDispatcher::Get().TakeItemFromPokemon(Pokemon)) {
        ItemTaken.Broadcast();
    } else {
        ItemNotTaken.Broadcast();
    }
}