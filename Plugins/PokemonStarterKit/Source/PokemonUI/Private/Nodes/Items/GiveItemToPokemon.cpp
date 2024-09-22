// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/GiveItemToPokemon.h"
#include "Blueprints/UtilityNodeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/Node/Utility_GiveItemToPokemon.h"

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
    auto Subsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UUtilityNodeSubsystem>();
    Subsystem->ExecuteUtilityFunction<UUtility_GiveItemToPokemon>(
        Item, Pokemon, PokemonIndex, FSimpleDelegate::CreateUObject(this, &UGiveItemToPokemon::ExecuteItemGiven),
        FSimpleDelegate::CreateUObject(this, &UGiveItemToPokemon::ExecuteItemRejected));
}

void UGiveItemToPokemon::ExecuteItemGiven() {
    ItemGiven.Broadcast();
    SetReadyToDestroy();
}

void UGiveItemToPokemon::ExecuteItemRejected() {
    ItemRejected.Broadcast();
    SetReadyToDestroy();
}