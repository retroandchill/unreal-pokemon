// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PauseMenuScreen.h"
#include "Data/Command.h"
#include "Windows/CommandWindow.h"

void UPauseMenuScreen::NativePreConstruct() {
    Super::NativePreConstruct();
    if (CommandWindow == nullptr)
        return;

    CommandWindow->GetOnCommandSelected().AddUniqueDynamic(this, &UPauseMenuScreen::ProcessCommand);
    CommandWindow->GetOnCancel().AddUniqueDynamic(this, &UPauseMenuScreen::CloseScreen);
    CommandWindow->SetIndex(0);
    CommandWindow->ActivateWidget();
}

void UPauseMenuScreen::NativeConstruct() {
    Super::NativeConstruct();
    PlaySound(OpenSound);
}

void UPauseMenuScreen::ProcessCommand(int32, UCommand *SelectedCommand) {
    SelectedCommand->ExecuteCommand(GetOwningPlayer());
}
