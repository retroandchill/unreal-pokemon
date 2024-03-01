// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"

#include "Windows/CommandWindow.h"

void UPauseMenuScreen::NativePreConstruct() {
	Super::NativePreConstruct();
	if (CommandWindow == nullptr)
		return;

	CommandWindow->OnCancel.AddDynamic(this, &UPauseMenuScreen::CloseScreen);
}
