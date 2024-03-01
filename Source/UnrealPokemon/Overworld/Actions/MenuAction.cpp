// "Unreal Pokémon" created by Retro & Chill.
#include "MenuAction.h"

#include "RPGPlayerController.h"
#include "Characters/GamePlayer.h"

void UMenuAction::PerformAction_Implementation(UObject* Owner) {
	auto Player = CastChecked<AGamePlayer>(Owner);
	auto Controller = CastChecked<ARPGPlayerController>(Player->GetController());
	Controller->AddScreenToStack(MenuClass);
}
