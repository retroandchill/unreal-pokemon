// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"
#include "Utilities/InteractiveUIUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UPauseMenuScreen)

UPauseMenuScreen *UPauseMenuScreen::AddPauseMenuScreenToStack(const UObject *WorldContextObject)
{
    return UInteractiveUIUtilities::InjectScreenToStack<UPauseMenuScreen>(WorldContextObject).GetPtrOrNull();
}

void UPauseMenuScreen::NativeConstruct()
{
    Super::NativeConstruct();
    PlaySound(OpenSound);
}
