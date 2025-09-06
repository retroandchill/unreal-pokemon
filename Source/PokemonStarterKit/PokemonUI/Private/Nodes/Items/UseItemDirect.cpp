// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/UseItemDirect.h"
#include "Field/FieldItemEffectDirect.h"

UUseItemDirect *UUseItemDirect::UseItemDirect(UBagScreen *Screen, FName Item, int32 Quantity)
{
    return ConstructUseItemNode<UUseItemDirect>(Screen, Item, Quantity);
}

UE5Coro::TCoroutine<> UUseItemDirect::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine)
{
    co_await UseItem<UFieldItemEffectDirect>();
}