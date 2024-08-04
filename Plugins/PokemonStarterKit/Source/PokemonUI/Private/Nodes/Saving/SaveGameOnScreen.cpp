// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/Saving/SaveGameOnScreen.h"
#include "Screens/SaveScreen.h"

USaveGameOnScreen * USaveGameOnScreen::SaveGame(USaveScreen *Screen) {
    auto Node = NewObject<USaveGameOnScreen>();
    Node->Screen = Screen;
    return Node;
}

void USaveGameOnScreen::Activate() {
    Screen->SaveGame(FOnSaveComplete::CreateWeakLambda(this, [this](bool bSuccess) {
        bSuccess ? SaveSuccess.Broadcast() : SaveFailed.Broadcast();
        SetReadyToDestroy();
    }));
}