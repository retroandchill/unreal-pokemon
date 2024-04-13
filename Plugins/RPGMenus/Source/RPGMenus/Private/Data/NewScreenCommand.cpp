// "Unreal Pokémon" created by Retro & Chill.

#include "Data/NewScreenCommand.h"
#include "RPGPlayerController.h"

void UNewScreenCommand::ExecuteCommand_Implementation(ARPGPlayerController *Controller) {
    Controller->AddScreenToStack(NextScreen);
}
