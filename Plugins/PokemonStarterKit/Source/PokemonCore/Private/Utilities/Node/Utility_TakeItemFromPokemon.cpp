// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/Node/Utility_TakeItemFromPokemon.h"

DEFINE_INJECTABLE_DEPENDENCY(UUtility_TakeItemFromPokemon)

void UUtility_TakeItemFromPokemon::Execute(const TScriptInterface<IPokemon> &Pokemon, FSimpleDelegate &&ItemTaken,
                                           FSimpleDelegate &&ItemNotTaken) {
    OnItemTaken.Add(MoveTemp(ItemTaken));
    OnItemNotTaken.Add(MoveTemp(ItemNotTaken));
    Execute(Pokemon);
}

void UUtility_TakeItemFromPokemon::ExecuteItemTaken() {
    OnItemTaken.Broadcast();
    Destruct();
}

void UUtility_TakeItemFromPokemon::ExecuteItemNotTaken() {
    OnItemNotTaken.Broadcast();
    Destruct();
}