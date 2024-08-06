// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/Items/UseItemOnPokemon.h"
#include "DataManager.h"
#include "Screens/BagScreen.h"
#include "Bag/Item.h"

UUseItemOnPokemon *UUseItemOnPokemon::UseItemOnPokemon(UBagScreen *Screen, FName Item, int32 Quantity,
                                                       const TScriptInterface<IPokemon> &Pokemon) {
    auto Node = NewObject<UUseItemOnPokemon>();
    Node->BagScreen = Screen;
    Node->Item = Item;
    Node->Quantity = Quantity;
    Node->Pokemon = Pokemon;
    return Node;
}

void UUseItemOnPokemon::Activate() {
    auto &ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(Item);
    BagScreen->TryUseItemOnPokemon(ItemData, Quantity, Pokemon, FOnItemEffectComplete::FDelegate::CreateWeakLambda(
                                       this, [this](bool bSuccess) {
                                           if (bSuccess) {
                                               ItemUsed.Broadcast();
                                           } else {
                                               ItemNotUsed.Broadcast();
                                           }
                                           SetReadyToDestroy();
                                       }));
}