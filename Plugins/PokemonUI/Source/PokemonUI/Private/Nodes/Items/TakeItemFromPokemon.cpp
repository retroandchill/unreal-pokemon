// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/TakeItemFromPokemon.h"
#include "Settings/BagSettings.h"
#include "Utilities/ItemUtilities.h"

UTakeItemFromPokemon *UTakeItemFromPokemon::TakeItemFromPokemon(const UObject *WorldContextObject,
                                                                const TScriptInterface<IPokemon> &Pokemon) {
    auto Node = NewObject<UTakeItemFromPokemon>();
    Node->WorldContextObject = WorldContextObject;
    Node->Pokemon = Pokemon;
    Node->ItemUtilities =
        NewObject<UObject>(Node, GetDefault<UBagSettings>()->GetItemUtilitiesClass().TryLoadClass<UObject>());
    return Node;
}

void UTakeItemFromPokemon::Activate() {
    check(ItemUtilities != nullptr)
    FItemResultNoRetValue OnSuccess;
    OnSuccess.BindDynamic(this, &UTakeItemFromPokemon::ExecuteItemTaken);
    FItemResultNoRetValue OnFailure;
    OnFailure.BindDynamic(this, &UTakeItemFromPokemon::ExecuteItemNotTaken);
    IItemUtilities::Execute_TakeItemFromPokemon(ItemUtilities, WorldContextObject, Pokemon, OnSuccess, OnFailure);
}

void UTakeItemFromPokemon::ExecuteItemTaken() {
    ItemTaken.Broadcast();
}

void UTakeItemFromPokemon::ExecuteItemNotTaken() {
    ItemNotTaken.Broadcast();
}