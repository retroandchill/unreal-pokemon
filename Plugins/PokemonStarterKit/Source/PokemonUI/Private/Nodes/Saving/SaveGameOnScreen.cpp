// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Saving/SaveGameOnScreen.h"
#include "Screens/SaveScreen.h"

USaveGameOnScreen *USaveGameOnScreen::SaveGame(USaveScreen *Screen) {
    auto Node = NewObject<USaveGameOnScreen>();
    Node->SetWorldContext(Screen);
    Node->Screen = Screen;
    return Node;
}

UE5Coro::TCoroutine<> USaveGameOnScreen::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    bool bSuccess = co_await Screen->SaveGame();
    bSuccess ? SaveSuccess.Broadcast() : SaveFailed.Broadcast();
}