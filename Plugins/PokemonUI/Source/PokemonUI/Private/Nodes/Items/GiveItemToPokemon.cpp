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
    return Node;
}

void UGiveItemToPokemon::Activate() {
    ItemUtilities = NewObject<UObject>(this, GetDefault<UBagSettings>()->GetItemUtilitiesClass());
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
    ItemUtilities = nullptr;
}

void UGiveItemToPokemon::ExecuteItemRejected() {
    ItemRejected.Broadcast();
    ItemUtilities = nullptr;
}