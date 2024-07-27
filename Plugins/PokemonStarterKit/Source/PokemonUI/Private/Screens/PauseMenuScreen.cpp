// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"
#include "Data/Command.h"
#include "Components/CommandWindow.h"

void UPauseMenuScreen::NativeConstruct() {
    Super::NativeConstruct();
    PlaySound(OpenSound);
}
