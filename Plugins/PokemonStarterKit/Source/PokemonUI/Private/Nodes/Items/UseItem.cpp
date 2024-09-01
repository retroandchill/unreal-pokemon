// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/Items/UseItem.h"

void UUseItem::OnItemEffectComplete(bool bSuccess) {
    if (bSuccess) {
        ItemUsed.Broadcast();
    } else {
        ItemNotUsed.Broadcast();
    }
    SetReadyToDestroy();
}