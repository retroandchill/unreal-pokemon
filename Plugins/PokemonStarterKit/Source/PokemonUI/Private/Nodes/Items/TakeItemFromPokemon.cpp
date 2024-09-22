// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/TakeItemFromPokemon.h"
#include "Blueprints/UtilityNodeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/Node/Utility_TakeItemFromPokemon.h"

UTakeItemFromPokemon *UTakeItemFromPokemon::TakeItemFromPokemon(const UObject *WorldContextObject,
                                                                const TScriptInterface<IPokemon> &Pokemon) {
    auto Node = NewObject<UTakeItemFromPokemon>();
    Node->WorldContextObject = WorldContextObject;
    Node->Pokemon = Pokemon;
    return Node;
}

void UTakeItemFromPokemon::Activate() {
    auto Subsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UUtilityNodeSubsystem>();
    Subsystem->ExecuteUtilityFunction<UUtility_TakeItemFromPokemon>(Pokemon,
        FSimpleDelegate::CreateUObject(this, &UTakeItemFromPokemon::ExecuteItemTaken),
        FSimpleDelegate::CreateUObject(this, &UTakeItemFromPokemon::ExecuteItemNotTaken));
}

void UTakeItemFromPokemon::ExecuteItemTaken() {
    ItemTaken.Broadcast();
    SetReadyToDestroy();
}

void UTakeItemFromPokemon::ExecuteItemNotTaken() {
    ItemNotTaken.Broadcast();
    SetReadyToDestroy();
}