// "Unreal Pokémon" created by Retro & Chill.
#include "Actions/MenuAction.h"

#include "RPGPlayerController.h"

void UMenuAction::PerformAction_Implementation(UObject* Owner) {
	auto Player = CastChecked<APawn>(Owner);
	auto Controller = CastChecked<ARPGPlayerController>(Player->GetController());
	Controller->AddScreenToStack(MenuClass);
}
