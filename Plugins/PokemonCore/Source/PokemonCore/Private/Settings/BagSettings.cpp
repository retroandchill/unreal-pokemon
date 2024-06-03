// "Unreal Pokémon" created by Retro & Chill.

#include "Settings/BagSettings.h"

int32 UBagSettings::GetMaxItemsPerSlot() const {
    return MaxItemsPerSlot;
}

const TMap<FName, FPocketInfo> & UBagSettings::GetPocketInfo() const {
    return PocketInfo;
}