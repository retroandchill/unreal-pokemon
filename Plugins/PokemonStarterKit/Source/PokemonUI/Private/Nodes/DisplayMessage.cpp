// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/PokemonUIAsyncActions.h"

UDisplayMessage *UDisplayMessage::DisplayMessage(const UObject *WorldContextObject, FText Message) {
    auto Node = NewObject<UDisplayMessage>();
    Node->WorldContextObject = WorldContextObject;
    Node->Message = std::move(Message);
    return Node;
}

UE5Coro::TCoroutine<> UDisplayMessage::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    co_await Pokemon::UI::DisplayMessage(WorldContextObject, Message);
    OnConfirm.Broadcast();
}
