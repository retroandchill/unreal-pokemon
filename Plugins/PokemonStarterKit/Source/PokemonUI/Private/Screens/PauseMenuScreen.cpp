// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"
#include "Data/Command.h"
#include "Windows/CommandWindow.h"

void UPauseMenuScreen::NativeConstruct() {
    Super::NativeConstruct();
    PlaySound(OpenSound);
}
