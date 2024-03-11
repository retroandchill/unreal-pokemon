// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GameNPC.h"

#include "GridUtils.h"


void AGameNPC::OnInteract_Implementation(AGameCharacter* Character) {
	if (auto OpposingDirection = GridBased2D::GetOpposingDirection(Character->GetDirection()); OpposingDirection.
		IsSet()) {
		FaceDirection(OpposingDirection.GetValue());
	}

	Interact.Broadcast();
}
