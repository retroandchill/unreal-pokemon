// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/ChooseItemFromBag.h"
#include "RetroLib/Optionals/IfPresentOrElse.h"
#include "Screens/BagScreen.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UChooseItemFromBag *UChooseItemFromBag::ChooseItemFromBag(const UObject *WorldContextObject,
                                                          const FItemFilter &ItemFilter)
{
    auto Node = NewObject<UChooseItemFromBag>();
    Node->SetWorldContext(WorldContextObject);
    Node->ItemFilter = ItemFilter;
    return Node;
}

UE5Coro::TCoroutine<> UChooseItemFromBag::ExecuteCoroutine(FForceLatentCoroutine)
{
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(GetWorldContext());
    Retro::Optionals::IfPresentOrElse(
        co_await Dispatcher.SelectItemFromBag(ItemFilter),
        [&](const FSelectedItemHandle &Handle) {
            OnSelected.Broadcast(Handle.GetScreen(), Handle.GetItem(), Handle.GetQuantity());
        },
        [this] { OnCanceled.Broadcast(); });
}