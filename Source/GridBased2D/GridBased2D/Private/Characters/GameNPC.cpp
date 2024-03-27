// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GameNPC.h"

#include "GridUtils.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"
#include "Interaction/InteractionType.h"


void AGameNPC::OnInteract_Implementation(const TScriptInterface<IGridMovable>& Character, EInteractionType InteractionType) {
	auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(Character.GetObject());
	if (auto OpposingDirection = UGridUtils::GetOpposingDirection(MovementComponent->GetDirection()); OpposingDirection.
		IsSet()) {
		FaceDirection(OpposingDirection.GetValue());
	}

	Interact.Broadcast(InteractionType);
}

uint8 AGameNPC::GetInteractionTypes() const {
	return static_cast<uint8>(EInteractionType::Talk);
}
