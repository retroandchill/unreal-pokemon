// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/Node/Utility_GiveItemToPokemon.h"

bool UUtility_GiveItemToPokemon::ImplementsGetWorld() const {
    return true;
}

void UUtility_GiveItemToPokemon::Execute(FName Item, const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex,
    FItemResult::FDelegate &&ItemGiven, FItemResult::FDelegate &&ItemNotGiven) {
    OnItemGiven.Add(MoveTemp(ItemGiven));
    OnItemNotGiven.Add(MoveTemp(ItemNotGiven));
    Execute(Item, Pokemon, PokemonIndex);
}

void UUtility_GiveItemToPokemon::ExecuteItemGiven() const {
    OnItemGiven.Broadcast();
}

void UUtility_GiveItemToPokemon::ExecuteItemNotGiven() const {
    OnItemNotGiven.Broadcast();
}