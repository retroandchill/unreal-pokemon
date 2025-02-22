﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Movement/MoveInDirection.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"

UMoveInDirection *UMoveInDirection::MoveInDirection(const TScriptInterface<IGridMovable> &Character,
                                                    EFacingDirection MovementDirection) {
    auto Node = NewObject<UMoveInDirection>();
    Node->SetWorldContext(Character.GetObject());
    Node->Character = Character;
    Node->MovementDirection = MovementDirection;
    return Node;
}

UE5Coro::TCoroutine<> UMoveInDirection::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine) {
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
    co_await MovementComponent->MoveInDirection(MovementDirection);
    if (PlayerController != nullptr && bCanInput) {
        Pawn->EnableInput(PlayerController);
    }

    OnMovementFinished.Broadcast();
}