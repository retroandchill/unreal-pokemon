// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/PokemonUIAsyncActions.h"

UDisplayMessage *UDisplayMessage::DisplayMessage(const UObject *WorldContextObject, FText Message) {
    auto Node = NewObject<UDisplayMessage>();
    Node->SetWorldContext(WorldContextObject);
    Node->Message = std::move(Message);
    return Node;
}

UE5Coro::TCoroutine<> UDisplayMessage::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    co_await Pokemon::UI::DisplayMessage(GetWorldContext(), Message);
    OnConfirm.Broadcast();
}
