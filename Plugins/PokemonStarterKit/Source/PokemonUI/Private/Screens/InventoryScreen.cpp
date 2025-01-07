// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/InventoryScreen.h"

// Add default functionality here for any IIInventoryScreen functions that are not pure virtual.
FSelectedItemHandle::FSelectedItemHandle(const TScriptInterface<IInventoryScreen> &InScreen, const FItem &InItem,
    int32 InQuantity) : Screen(InScreen), Item(InItem), Quantity(InQuantity) {}