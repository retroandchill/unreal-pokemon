// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/UseItemOnPokemon.h"
#include "Field/FieldItemEffectUseOnPokemon.h"

UUseItemOnPokemon *UUseItemOnPokemon::UseItemOnPokemon(UBagScreen *Screen, FName Item, int32 Quantity,
                                                       const TScriptInterface<IPokemon> &Pokemon)
{
    auto Node = ConstructUseItemNode<UUseItemOnPokemon>(Screen, Item, Quantity);
    Node->Pokemon = Pokemon;
    return Node;
}

UE5Coro::TCoroutine<> UUseItemOnPokemon::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine)
{
    co_await UseItem<UFieldItemEffectUseOnPokemon>({}, Pokemon);
}