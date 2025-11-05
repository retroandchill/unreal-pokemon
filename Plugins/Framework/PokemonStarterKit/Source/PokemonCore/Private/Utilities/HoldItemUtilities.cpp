// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/HoldItemUtilities.h"
#include "Pokemon/Pokemon.h"

void UHoldItemUtilities::GetHeldItem(const TScriptInterface<IPokemon> &Pokemon, FItem &HeldItem,
                                     EHoldItemResult &Branches) {
    using enum EHoldItemResult;

    if (Pokemon == nullptr) {
        UE_LOG(LogBlueprint, Error, TEXT("Get Held Item called on an invalid Pokémon reference!"))
        Branches = NoHeldItem;
        return;
    }

    if (auto FoundHeldItem = Pokemon->GetHoldItem(); FoundHeldItem.IsSet()) {
        Branches = HasItem;
        HeldItem = *FoundHeldItem;
    } else {
        Branches = NoHeldItem;
    }
}