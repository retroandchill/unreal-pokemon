// "Unreal Pokémon" created by Retro & Chill.

#include "Movement/MoveInDirection.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"

UMoveInDirection *UMoveInDirection::MoveInDirection(const TScriptInterface<IGridMovable> &Character,
                                                    EFacingDirection MovementDirection) {
    auto Node = NewObject<UMoveInDirection>();
    Node->Character = Character;
    Node->MovementDirection = MovementDirection;
    return Node;
}

void UMoveInDirection::Activate() {
    bool bCanInput = false;
    APlayerController *PlayerController = nullptr;
    auto Pawn = Cast<APawn>(Character.GetObject());
    if (Pawn != nullptr) {
        PlayerController = Cast<APlayerController>(Pawn->Controller);
        if (PlayerController != nullptr) {
            bCanInput = PlayerController->InputEnabled();
            Pawn->DisableInput(PlayerController);
        }
    }

    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(Character.GetObject());
    MovementComponent->MoveInDirection(MovementDirection, [this, PlayerController, bCanInput, Pawn] {
        if (PlayerController != nullptr && bCanInput) {
            Pawn->EnableInput(PlayerController);
        }

        OnMovementFinished.Broadcast();
        SetReadyToDestroy();
    });
}