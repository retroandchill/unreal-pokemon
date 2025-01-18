// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UDisplayMessage *UDisplayMessage::DisplayMessage(const UObject *WorldContextObject, FText Message) {
    auto Node = NewObject<UDisplayMessage>();
    Node->SetWorldContext(WorldContextObject);
    Node->Message = std::move(Message);
    return Node;
}

UE5Coro::TCoroutine<> UDisplayMessage::ExecuteCoroutine(FForceLatentCoroutine) {
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get();
    co_await Dispatcher.DisplayMessage(Message);
    OnConfirm.Broadcast();
}
