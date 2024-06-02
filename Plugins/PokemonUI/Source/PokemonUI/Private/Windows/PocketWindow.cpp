// "Unreal Pokémon" created by Retro & Chill.


#include "Windows/PocketWindow.h"

FName UPocketWindow::GetCurrentPocket() const {
    return CurrentPocket;
}

void UPocketWindow::SetCurrentPocket(FName Pocket) {
    CurrentPocket = Pocket;
    UpdatePocketInfo();
}

void UPocketWindow::UpdatePocketInfo_Implementation() {
    
}