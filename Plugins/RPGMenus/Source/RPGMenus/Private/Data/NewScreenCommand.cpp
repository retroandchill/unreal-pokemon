// "Unreal Pokémon" created by Retro & Chill.

#include "Data/NewScreenCommand.h"
#include "RPGMenusSubsystem.h"

void UNewScreenCommand::ExecuteCommand_Implementation(APlayerController *Controller) {
    Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->AddScreenToStack(NextScreen);
}
