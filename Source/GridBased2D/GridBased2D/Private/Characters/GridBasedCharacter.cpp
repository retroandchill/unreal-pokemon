// "Unreal Pokémon" created by Retro & Chill.


#include "Characters/GridBasedCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridUtils.h"
#include "InputAction.h"
#include "MathUtilities.h"
#include "Actions/Action.h"
#include "Components/CapsuleComponent.h"
#include "Components/GridBasedMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractionType.h"

AGridBasedCharacter::AGridBasedCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	auto GridSize = static_cast<float>(UGridUtils::GetGridSize());
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleRadius(GridSize / 2);
	Capsule->SetCapsuleHalfHeight(GridSize / 2);

	GridBasedMovementComponent = CreateDefaultSubobject<UGridBasedMovementComponent>(TEXT("GridBasedMovement"));
}

// Called when the game starts or when spawned
void AGridBasedCharacter::BeginPlay() {
	Super::BeginPlay();

	if (const auto* const PlayerController = Cast<APlayerController>(Controller); PlayerController != nullptr) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
	}
}

void AGridBasedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	using enum ETriggerEvent;

	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveInput.Get(), Triggered, this, &AGridBasedCharacter::Move);
	Input->BindAction(FaceDirectionInput.Get(), Triggered, this, &AGridBasedCharacter::Turn);
	Input->BindAction(InteractInput.Get(), Triggered, this, &AGridBasedCharacter::Interact);
	Input->BindAction(PauseInput.Get(), Triggered, this, &AGridBasedCharacter::PauseGame);
}

UGridBasedMovementComponent* AGridBasedCharacter::GetGridBasedMovementComponent_Implementation() const {
	return GridBasedMovementComponent;
}

void AGridBasedCharacter::OnInteract_Implementation(const TScriptInterface<IGridMovable>& Character,
	EInteractionType InteractionType) {
	OnInteractedWith.Broadcast(InteractionType);
}

uint8 AGridBasedCharacter::GetInteractionTypes() const {
	return InteractionTypes;
}

UGridBasedMovementComponent* AGridBasedCharacter::GetGridBasedMovementComponentInternal() const {
	return GridBasedMovementComponent;
}

bool AGridBasedCharacter::InvalidFloor(const FVector& TargetSquare) const {
	auto Edge = FindLocationJustOffTileEdge(TargetSquare);
	FFindFloorResult Result;
	GetCharacterMovement()->FindFloor(Edge, Result, true);
	return !Result.bWalkableFloor;
}

bool AGridBasedCharacter::IsStandingNextToCliff(const FVector& TargetSquare) const {
	double Distance1 = GetDistanceToGround(FindLocationJustOffTileEdge(TargetSquare));
	double Distance2 = GetDistanceToGround(UMathUtilities::Midpoint(GetActorLocation(), TargetSquare));
	return FMath::Abs(Distance1 - Distance2) > GetCharacterMovement()->MaxStepHeight;
}

FVector AGridBasedCharacter::FindLocationJustOffTileEdge(const FVector& TargetSquare) const {
	auto Location = GetActorLocation();
	auto MidPoint = UMathUtilities::Midpoint(TargetSquare, Location);
	auto Diff = TargetSquare - Location;
	Diff.Normalize();
	return MidPoint + Diff;
}

double AGridBasedCharacter::GetDistanceToGround(const FVector& Position) const {
	static constexpr double TraceMax = 100.0;
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	auto Hit = GetWorld()->LineTraceSingleByChannel(Result, Position, Position - FVector(0, 0, TraceMax), ECC_Visibility, Params);
	return Hit ? Result.Distance : TraceMax;
}

void AGridBasedCharacter::Move(const FInputActionInstance& Input) {
	auto Vector = Input.GetValue().Get<FVector2D>();
	auto Dir = UGridUtils::VectorToFacingDirection(Vector);
	if (!Dir.IsSet() || GridBasedMovementComponent->GetCurrentPosition() != GridBasedMovementComponent->
		GetDesiredPosition())
		return;

	GridBasedMovementComponent->MoveInDirection(Dir.GetValue());
}

void AGridBasedCharacter::Turn(const FInputActionInstance& Input) {
	auto Vector = Input.GetValue().Get<FVector2D>();
	auto Dir = UGridUtils::VectorToFacingDirection(Vector);
	if (!Dir.IsSet() || GridBasedMovementComponent->GetCurrentPosition() != GridBasedMovementComponent->
		GetDesiredPosition())
		return;

	GridBasedMovementComponent->FaceDirection(Dir.GetValue());
}

void AGridBasedCharacter::Interact() {
	for (auto Results = GridBasedMovementComponent->HitTestOnFacingTile(
		     GridBasedMovementComponent->GetDirection()); auto& Result : Results) {
		if (auto Interactable = Cast<IInteractable>(Result.GetActor()); Interactable == nullptr ||
			(Interactable->GetInteractionTypes() & static_cast<uint8>(EInteractionType::Talk)) == 0)
			continue;

		IInteractable::Execute_OnInteract(Result.GetActor(), this, EInteractionType::Talk);
	}
}

void AGridBasedCharacter::PauseGame() {
	PauseAction->PerformAction(this);
}
