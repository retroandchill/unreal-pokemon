// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GameNPC.h"

#include "GridUtils.h"
#include "Interaction/InteractionType.h"


void AGameNPC::OnInteract_Implementation(AGameCharacter* Character, EInteractionType InteractionType) {
	if (auto OpposingDirection = GridBased2D::GetOpposingDirection(Character->GetDirection()); OpposingDirection.
		IsSet()) {
		FaceDirection(OpposingDirection.GetValue());
	}

	Interact.Broadcast(InteractionType);
}

uint8 AGameNPC::GetInteractionTypes() const {
	return static_cast<uint8>(EInteractionType::Talk);
}
