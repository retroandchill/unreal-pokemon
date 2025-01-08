// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/ChooseItemFromBag.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "RetroLib/Optionals/IfPresentOrElse.h"
#include "Screens/BagScreen.h"
#include "Utilities/PokemonUIAsyncActions.h"

UChooseItemFromBag *UChooseItemFromBag::ChooseItemFromBag(const UObject *WorldContextObject,
                                                          const FItemFilter &ItemFilter) {
    auto Node = NewObject<UChooseItemFromBag>();
    Node->SetWorldContext(WorldContextObject);
    Node->ItemFilter = ItemFilter;
    return Node;
}

UE5Coro::TCoroutine<> UChooseItemFromBag::ExecuteCoroutine(FForceLatentCoroutine) {
    Retro::Optionals::IfPresentOrElse(
        co_await Pokemon::UI::SelectItemFromBag(GetWorldContext(), ItemFilter),
        [&](const FSelectedItemHandle &Handle) {
            OnSelected.Broadcast(Handle.GetScreen(), Handle.GetItem(), Handle.GetQuantity());
        },
        [this] { OnCanceled.Broadcast(); }
    );
}