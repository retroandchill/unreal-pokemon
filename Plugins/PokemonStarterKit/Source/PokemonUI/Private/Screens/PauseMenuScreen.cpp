// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"
#include "Components/CommandWindow.h"
#include "Data/Command.h"

void UPauseMenuScreen::NativeConstruct() {
    Super::NativeConstruct();
    PlaySound(OpenSound);
}
