// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/Items/GiveItemToPokemon.h"
#include "Settings/BagSettings.h"
#include "Utilities/ItemUtilities.h"

UGiveItemToPokemon * UGiveItemToPokemon::GiveItemToPokemon(const UObject *WorldContextObject, FName Item,
                                                           const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex) {
    auto Node = NewObject<UGiveItemToPokemon>();
    Node->WorldContextObject = WorldContextObject;
    Node->Item = Item;
    Node->Pokemon = Pokemon;
    Node->PokemonIndex = PokemonIndex;
    Node->ItemUtilities = NewObject<UObject>(Node, GetDefault<UBagSettings>()->GetItemUtilitiesClass().TryLoadClass<UObject>());
    return Node;
}

void UGiveItemToPokemon::Activate() {
    check(ItemUtilities != nullptr)
    FItemResultNoRetValue OnSuccess;
    OnSuccess.BindDynamic(this, &UGiveItemToPokemon::ExecuteItemGiven);
    FItemResultNoRetValue OnFailure;
    OnFailure.BindDynamic(this, &UGiveItemToPokemon::ExecuteItemRejected);
    IItemUtilities::Execute_GiveItemToPokemon(ItemUtilities, WorldContextObject, Item, Pokemon, PokemonIndex,
        OnSuccess, OnFailure);
}

void UGiveItemToPokemon::ExecuteItemGiven() {
    ItemGiven.Broadcast();
}

void UGiveItemToPokemon::ExecuteItemRejected() {
    ItemRejected.Broadcast();
}