// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/GiveItemToPokemon.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/ItemUtilities.h"
#include "Utilities/UtilitiesSubsystem.h"

UGiveItemToPokemon *UGiveItemToPokemon::GiveItemToPokemon(const UObject *WorldContextObject, FItemHandle Item,
                                                          const TScriptInterface<IPokemon> &Pokemon,
                                                          int32 PokemonIndex) {
    auto Node = NewObject<UGiveItemToPokemon>();
    Node->WorldContextObject = WorldContextObject;
    Node->Item = Item;
    Node->Pokemon = Pokemon;
    Node->PokemonIndex = PokemonIndex;
    return Node;
}

void UGiveItemToPokemon::Activate() {
    auto ItemUtilities =
        UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UUtilitiesSubsystem>()->GetItemUtilities();
    check(ItemUtilities != nullptr)
    FItemResultNoRetValue OnSuccess;
    OnSuccess.BindDynamic(this, &UGiveItemToPokemon::ExecuteItemGiven);
    FItemResultNoRetValue OnFailure;
    OnFailure.BindDynamic(this, &UGiveItemToPokemon::ExecuteItemRejected);
    IItemUtilities::Execute_GiveItemToPokemon(ItemUtilities, WorldContextObject, Item, Pokemon, PokemonIndex, OnSuccess,
                                              OnFailure);
}

void UGiveItemToPokemon::ExecuteItemGiven() {
    ItemGiven.Broadcast();
    SetReadyToDestroy();
}

void UGiveItemToPokemon::ExecuteItemRejected() {
    ItemRejected.Broadcast();
    SetReadyToDestroy();
}