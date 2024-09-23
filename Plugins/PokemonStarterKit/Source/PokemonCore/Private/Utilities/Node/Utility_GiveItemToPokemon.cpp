// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/Node/Utility_GiveItemToPokemon.h"

void UUtility_GiveItemToPokemon::Execute(const FItemHandle &Item, const TScriptInterface<IPokemon> &Pokemon,
                                         int32 PokemonIndex, FSimpleDelegate &&ItemGiven,
                                         FSimpleDelegate &&ItemNotGiven) {
    OnItemGiven.Add(MoveTemp(ItemGiven));
    OnItemNotGiven.Add(MoveTemp(ItemNotGiven));
    Execute(Item, Pokemon, PokemonIndex);
}

void UUtility_GiveItemToPokemon::ExecuteItemGiven() {
    OnItemGiven.Broadcast();
    Destruct();
}

void UUtility_GiveItemToPokemon::ExecuteItemNotGiven() {
    OnItemNotGiven.Broadcast();
    Destruct();
}