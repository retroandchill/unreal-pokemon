// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/TakeItemFromPokemon.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/ItemUtilities.h"
#include "Utilities/UtilitiesSubsystem.h"

UTakeItemFromPokemon *UTakeItemFromPokemon::TakeItemFromPokemon(const UObject *WorldContextObject,
                                                                const TScriptInterface<IPokemon> &Pokemon) {
    auto Node = NewObject<UTakeItemFromPokemon>();
    Node->WorldContextObject = WorldContextObject;
    Node->Pokemon = Pokemon;
    return Node;
}

void UTakeItemFromPokemon::Activate() {
    auto ItemUtilities = UGameplayStatics::GetGameInstance(WorldContextObject)
                             ->GetSubsystem<UUtilitiesSubsystem>()
                             ->GetItemUtilities();
    check(ItemUtilities != nullptr)
    FItemResultNoRetValue OnSuccess;
    OnSuccess.BindDynamic(this, &UTakeItemFromPokemon::ExecuteItemTaken);
    FItemResultNoRetValue OnFailure;
    OnFailure.BindDynamic(this, &UTakeItemFromPokemon::ExecuteItemNotTaken);
    IItemUtilities::Execute_TakeItemFromPokemon(ItemUtilities, WorldContextObject, Pokemon, OnSuccess, OnFailure);
}

void UTakeItemFromPokemon::ExecuteItemTaken() {
    ItemTaken.Broadcast();
    SetReadyToDestroy();
}

void UTakeItemFromPokemon::ExecuteItemNotTaken() {
    ItemNotTaken.Broadcast();
    SetReadyToDestroy();
}