// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Items/UseItemDirect.h"
#include "Field/FieldItemEffectDirect.h"

UUseItemDirect *UUseItemDirect::UseItemDirect(UBagScreen *Screen, FName Item, int32 Quantity) {
    return ConstructUseItemNode<UUseItemDirect>(Screen, Item, Quantity);
}

void UUseItemDirect::Activate() {
    UseItem<UFieldItemEffectDirect>();
}