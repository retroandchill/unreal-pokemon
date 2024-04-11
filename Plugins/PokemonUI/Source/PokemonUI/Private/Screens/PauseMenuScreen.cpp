// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"

#include "Data/Command.h"
#include "RPGPlayerController.h"
#include "Windows/CommandWindow.h"

void UPauseMenuScreen::NativePreConstruct() {
    Super::NativePreConstruct();
    if (CommandWindow == nullptr)
        return;

    CommandWindow->OnCommandSelected.AddDynamic(this, &UPauseMenuScreen::ProcessCommand);
    CommandWindow->OnCancel.AddDynamic(this, &UPauseMenuScreen::CloseScreen);
}

void UPauseMenuScreen::NativeConstruct() {
    Super::NativeConstruct();
    PlaySound(OpenSound);
}

void UPauseMenuScreen::ProcessCommand(int32, UCommand *SelectedCommand) {
    auto PlayerController = Cast<ARPGPlayerController>(GetOwningPlayer());
    if (PlayerController == nullptr)
        return;

    SelectedCommand->ExecuteCommand(PlayerController);
}
