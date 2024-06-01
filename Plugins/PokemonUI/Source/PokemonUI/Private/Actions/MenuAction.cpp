// "Unreal Pokémon" created by Retro & Chill.
#include "Actions/MenuAction.h"
#include "RPGMenusSubsystem.h"

void UMenuAction::PerformAction_Implementation(UObject *Owner) {
    Super::PerformAction_Implementation(Owner);
    auto Player = CastChecked<APawn>(Owner);
    auto Controller = CastChecked<APlayerController>(Player->GetController());
    Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->AddScreenToStack(MenuClass);
}
