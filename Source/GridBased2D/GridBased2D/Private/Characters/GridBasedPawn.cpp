// "Unreal Pokémon" created by Retro & Chill.


#include "Characters/GridBasedPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridUtils.h"
#include "InputAction.h"
#include "Actions/Action.h"
#include "Components/GridBasedMovementComponent.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractionType.h"

AGridBasedPawn::AGridBasedPawn() {
	PrimaryActorTick.bCanEverTick = true;

	GridBasedMovementComponent = CreateDefaultSubobject<UGridBasedMovementComponent>(TEXT("GridBasedMovement"));
}

// Called when the game starts or when spawned
void AGridBasedPawn::BeginPlay() {
	Super::BeginPlay();

	if (const auto* const PlayerController = Cast<APlayerController>(Controller); PlayerController != nullptr) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
	}
}

void AGridBasedPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	using enum ETriggerEvent;

	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveInput.Get(), Triggered, this, &AGridBasedPawn::Move);
	Input->BindAction(FaceDirectionInput.Get(), Triggered, this, &AGridBasedPawn::Turn);
	Input->BindAction(InteractInput.Get(), Triggered, this, &AGridBasedPawn::Interact);
	Input->BindAction(PauseInput.Get(), Triggered, this, &AGridBasedPawn::PauseGame);
}

UGridBasedMovementComponent* AGridBasedPawn::GetGridBasedMovementComponent_Implementation() const {
	return GridBasedMovementComponent;
}

void AGridBasedPawn::OnInteract_Implementation(const TScriptInterface<IGridMovable>& Character,
	EInteractionType InteractionType) {
	OnInteractedWith.Broadcast(InteractionType);
}

uint8 AGridBasedPawn::GetInteractionTypes() const {
	return InteractionTypes;
}

void AGridBasedPawn::Move(const FInputActionInstance& Input) {
	auto Vector = Input.GetValue().Get<FVector2D>();
	auto Dir = UGridUtils::VectorToFacingDirection(Vector);
	if (!Dir.IsSet() || GridBasedMovementComponent->GetCurrentPosition() != GridBasedMovementComponent->
		GetDesiredPosition())
		return;

	GridBasedMovementComponent->MoveInDirection(Dir.GetValue());
}

void AGridBasedPawn::Turn(const FInputActionInstance& Input) {
	auto Vector = Input.GetValue().Get<FVector2D>();
	auto Dir = UGridUtils::VectorToFacingDirection(Vector);
	if (!Dir.IsSet() || GridBasedMovementComponent->GetCurrentPosition() != GridBasedMovementComponent->
		GetDesiredPosition())
		return;

	GridBasedMovementComponent->FaceDirection(Dir.GetValue());
}

void AGridBasedPawn::Interact() {
	for (auto Results = GridBasedMovementComponent->HitTestOnFacingTile(
		     GridBasedMovementComponent->GetDirection()); auto& Result : Results) {
		if (auto Interactable = Cast<IInteractable>(Result.GetActor()); Interactable == nullptr ||
			(Interactable->GetInteractionTypes() & static_cast<uint8>(EInteractionType::Talk)) == 0)
			continue;

		IInteractable::Execute_OnInteract(Result.GetActor(), this, EInteractionType::Talk);
	}
}

void AGridBasedPawn::PauseGame() {
	PauseAction->PerformAction(this);
}
