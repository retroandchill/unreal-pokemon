// "Unreal Pokémon" created by Retro & Chill.


#include "Movement/MoveInDirection.h"

#include "Characters/GameCharacter.h"

UMoveInDirection* UMoveInDirection::MoveInDirection(AGameCharacter* Character, EFacingDirection MovementDirection) {
	auto Node = NewObject<UMoveInDirection>();
	Node->Character = Character;
	Node->MovementDirection = MovementDirection;
	return Node;
}

void UMoveInDirection::Activate() {
	auto PlayerController = Cast<APlayerController>(Character->Controller);
	bool bCanInput = false;
	if (PlayerController != nullptr) {
		bCanInput = PlayerController->InputEnabled();
		Character->DisableInput(PlayerController);
	}

	Character->MoveInDirection(MovementDirection, [this, PlayerController, bCanInput]() {
		if (PlayerController != nullptr && bCanInput) {
			Character->EnableInput(PlayerController);
		}
		
		OnMovementFinished.Broadcast();
	});
}