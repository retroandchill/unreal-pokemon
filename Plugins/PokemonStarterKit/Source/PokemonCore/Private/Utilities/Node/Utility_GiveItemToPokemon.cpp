// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/Node/Utility_GiveItemToPokemon.h"

DEFINE_INJECTABLE_DEPENDENCY(UUtility_GiveItemToPokemon)

void UUtility_GiveItemToPokemon::Execute(const FItemHandle &Item, const TScriptInterface<IPokemon> &Pokemon,
                                         int32 PokemonIndex, FSimpleDelegate &&ItemGiven,
                                         FSimpleDelegate &&ItemNotGiven) {
    OnItemGiven.Add(std::move(ItemGiven));
    OnItemNotGiven.Add(std::move(ItemNotGiven));
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