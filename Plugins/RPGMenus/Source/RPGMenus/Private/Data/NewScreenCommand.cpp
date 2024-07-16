// "Unreal Pokémon" created by Retro & Chill.

#include "Data/NewScreenCommand.h"
#include "PrimaryGameLayout.h"
#include "Screens/Screen.h"

void UNewScreenCommand::ExecuteCommand_Implementation(APlayerController *Controller) {
    UPrimaryGameLayout::GetPrimaryGameLayout(Controller)
        ->PushWidgetToLayerStack(RPG::Menus::PrimaryMenuLayerTag, NextScreen);
}
