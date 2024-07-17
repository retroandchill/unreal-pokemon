// "Unreal Pokémon" created by Retro & Chill.

#include "Characters/GridBasedCharacter.h"
#include "Actions/Action.h"
#include "Components/CapsuleComponent.h"
#include "Components/GridBasedMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GridUtils.h"
#include "InputAction.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractionType.h"
#include "MathUtilities.h"

AGridBasedCharacter::AGridBasedCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    auto GridSize = static_cast<float>(UGridUtils::GetDefaultGridSize());
    auto Capsule = GetCapsuleComponent();
    Capsule->SetCapsuleRadius(GridSize / 2);
    Capsule->SetCapsuleHalfHeight(GridSize / 2);

    GridBasedMovementComponent = CreateDefaultSubobject<UGridBasedMovementComponent>(TEXT("GridBasedMovement"));
}

// Called when the game starts or when spawned
void AGridBasedCharacter::BeginPlay() {
    Super::BeginPlay();

    if (const auto *const PlayerController = Cast<APlayerController>(Controller); PlayerController != nullptr) {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AGridBasedCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    using enum ETriggerEvent;

    auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    Input->BindAction(MoveInput.Get(), Triggered, this, &AGridBasedCharacter::Move);
    Input->BindAction(FaceDirectionInput.Get(), Triggered, this, &AGridBasedCharacter::Turn);
    Input->BindAction(InteractInput.Get(), Triggered, this, &AGridBasedCharacter::Interact);
    Input->BindAction(PauseInput.Get(), Triggered, this, &AGridBasedCharacter::PauseGame);
}

UGridBasedMovementComponent *AGridBasedCharacter::GetGridBasedMovementComponent_Implementation() const {
    return GridBasedMovementComponent;
}

void AGridBasedCharacter::OnInteract_Implementation(const TScriptInterface<IGridMovable> &Character,
                                                    EInteractionType InteractionType) {
    OnInteractedWith.Broadcast(InteractionType);
}

bool AGridBasedCharacter::PerformAdditionalMovementChecks_Implementation(const FVector &TargetSquare, bool bBlockingHit,
                                                                         UPrimitiveComponent *HitComponent) {
    return bBlockingHit;
}

uint8 AGridBasedCharacter::GetInteractionTypes() const {
    return InteractionTypes;
}

UGridBasedMovementComponent *AGridBasedCharacter::GetGridBasedMovementComponentInternal() const {
    return GridBasedMovementComponent;
}

void AGridBasedCharacter::Move(const FInputActionInstance &Input) {
    auto Vector = Input.GetValue().Get<FVector2D>();
    auto Dir = UGridUtils::VectorToFacingDirection(Vector);
    if (!Dir.IsSet() ||
        GridBasedMovementComponent->GetCurrentPosition() != GridBasedMovementComponent->GetDesiredPosition())
        return;

    GridBasedMovementComponent->MoveInDirection(Dir.GetValue());
}

void AGridBasedCharacter::Turn(const FInputActionInstance &Input) {
    auto Vector = Input.GetValue().Get<FVector2D>();
    auto Dir = UGridUtils::VectorToFacingDirection(Vector);
    if (!Dir.IsSet() ||
        GridBasedMovementComponent->GetCurrentPosition() != GridBasedMovementComponent->GetDesiredPosition())
        return;

    GridBasedMovementComponent->FaceDirection(Dir.GetValue());
}

void AGridBasedCharacter::Interact() {
    for (auto Results = GridBasedMovementComponent->HitTestOnFacingTile(GridBasedMovementComponent->GetDirection());
         const auto &Result : Results) {
        if (auto Interactable = Cast<IInteractable>(Result.GetActor());
            Interactable == nullptr || (static_cast<std::byte>(Interactable->GetInteractionTypes()) &
                                        static_cast<std::byte>(EInteractionType::Talk)) == static_cast<std::byte>(0))
            continue;

        Execute_OnInteract(Result.GetActor(), this, EInteractionType::Talk);
    }
}

void AGridBasedCharacter::PauseGame() {
    PauseAction->PerformAction(this);
}
