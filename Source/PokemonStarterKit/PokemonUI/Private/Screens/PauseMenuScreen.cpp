// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"
#include "Utilities/RPGMenuUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UPauseMenuScreen)

UPauseMenuScreen *UPauseMenuScreen::AddPauseMenuScreenToStack(const UObject *WorldContextObject)
{
    return URPGMenuUtilities::InjectScreenToStack<UPauseMenuScreen>(WorldContextObject).GetPtrOrNull();
}

void UPauseMenuScreen::NativeConstruct()
{
    Super::NativeConstruct();
    PlaySound(OpenSound);
}
