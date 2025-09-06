// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/GiveItemToPokemon.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UGiveItemToPokemon *UGiveItemToPokemon::GiveItemToPokemon(const UObject *WorldContextObject, FItemHandle Item,
                                                          const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex)
{
    auto Node = NewObject<UGiveItemToPokemon>();
    Node->SetWorldContext(WorldContextObject);
    Node->Item = Item;
    Node->Pokemon = Pokemon;
    Node->PokemonIndex = PokemonIndex;
    return Node;
}

UE5Coro::TCoroutine<> UGiveItemToPokemon::ExecuteCoroutine(FForceLatentCoroutine)
{
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(GetWorldContext());
    auto bGiven = co_await Dispatcher.GiveItemToPokemon(Item, Pokemon, PokemonIndex);
    bGiven ? ItemGiven.Broadcast() : ItemRejected.Broadcast();
}