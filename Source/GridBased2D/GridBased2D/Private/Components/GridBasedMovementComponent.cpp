// "Unreal Pokémon" created by Retro & Chill.


#include "Components/GridBasedMovementComponent.h"

#include "GridUtils.h"
#include "MathUtilities.h"
#include "Characters/MoveCheckResult.h"
#include "Components/GridBasedAnimationComponent.h"
#include "Components/GridMovable.h"
#include "Interaction/Interactable.h"
#include "Map/GridBasedMap.h"
#include "Map/MapSubsystem.h"


UGridBasedMovementComponent::UGridBasedMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UGridBasedMovementComponent::BeginPlay() {
	Super::BeginPlay();

	auto Owner = GetOwner();
	GUARD(Owner == nullptr,)
	auto Position = Owner->GetActorLocation();
	CurrentPosition.X = FMath::FloorToInt32(Position.X / UGridUtils::GetGridSize());
	CurrentPosition.Y = FMath::FloorToInt32(Position.Y / UGridUtils::GetGridSize());
	DesiredPosition = CurrentPosition;
	Position.X = CurrentPosition.X * UGridUtils::GetGridSize();
	Position.Y = CurrentPosition.Y * UGridUtils::GetGridSize();
	GetOwner()->SetActorLocation(Position);

	auto Pawn = Cast<APawn>(Owner);
	GUARD(Pawn == nullptr || !Pawn->IsPlayerControlled(), )
	if (auto MapSubsystem = Owner->GetGameInstance()->GetSubsystem<UMapSubsystem>(); MapSubsystem != nullptr
		&& Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass())) {
		MapSubsystem->SetPlayerLocation(Owner);
	}
}

#if WITH_EDITOR
void UGridBasedMovementComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	auto AnimationComponent = GetGridBasedAnimationComponent();
	GUARD(AnimationComponent == nullptr, )
	AnimationComponent->UpdateDirection(Direction);
}
#endif

// Called every frame
void UGridBasedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMovement(DeltaTime);
	UpdateAnimation(DeltaTime);
}

TScriptInterface<IGridBasedAnimationComponent> UGridBasedMovementComponent::GetGridBasedAnimationComponent() const {
	return GridBasedAnimationComponent;
}

void UGridBasedMovementComponent::SetGridBasedAnimationComponent(TScriptInterface<IGridBasedAnimationComponent> NewGridBasedAnimationComponent) {
	GridBasedAnimationComponent = NewGridBasedAnimationComponent;
}

void UGridBasedMovementComponent::MoveInDirection(EFacingDirection MovementDirection) {
	FaceDirection(MovementDirection);
	if (auto [bCanMove, FoundActors] = MovementCheck(MovementDirection); !bCanMove) {
		if (MoveCallback.IsSet()) {
			auto Callback = MoveTemp(MoveCallback.GetValue());
			MoveCallback.Reset();
			Callback();
		}
		HitInteraction(FoundActors);
		return;
	}

	UGridUtils::AdjustMovementPosition(MovementDirection, DesiredPosition);

	MoveTimer.Emplace(0.f);
	StopTimer.Reset();
}

void UGridBasedMovementComponent::MoveInDirection(EFacingDirection MovementDirection,
                                                  TFunction<void()>&& MovementCompleteCallback) {
	GUARD_WARN(MoveCallback.IsSet(), , TEXT("The movement timer is already set for character: %s"),
	           *GetOwner()->GetName())

	MoveCallback.Emplace(MoveTemp(MovementCompleteCallback));
	MoveInDirection(MovementDirection);
}

FMoveCheckResult UGridBasedMovementComponent::MovementCheck(EFacingDirection MovementDirection) const {
	FMoveCheckResult Ret;
	TArray<AActor*> Maps;
	UGameplayStatics::GetAllActorsWithInterface(this, UMapGrid::StaticClass(), Maps);
	auto DestinationPosition = GetCurrentPosition();
	UGridUtils::AdjustMovementPosition(MovementDirection, DestinationPosition);
	bool bMapFound = false;
	for (auto Actor : Maps) {
		if (TScriptInterface<IMapGrid> Map = Actor; Map->IsPositionInMap(DestinationPosition)) {
			bMapFound = Map->IsObjectInMap(GetOwner()) || CanMoveBetweenMaps();
			break;
		}
	}

	if (!bMapFound) {
		Ret.bCanMove = false;
		return Ret;
	}

	for (auto Results = HitTestOnFacingTile(MovementDirection); auto& Result : Results) {
		if (Result.bBlockingHit) {
			Ret.bCanMove = false;
		}

		if (auto Interactable = Cast<IInteractable>(Result.GetActor()); Interactable != nullptr) {
			Ret.FoundActors.Emplace(Result.GetActor());
		}
	}
	
	if (auto Owner = GetOwner(); Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass())) {
		FVector LocalOffset(0, 0, 0);
		UGridUtils::AdjustMovementPosition(MovementDirection, LocalOffset);
		auto Position = Owner->GetActorLocation();
		auto GridPosition = LocalOffset * UGridUtils::GetGridSize() + Position;
		Ret.bCanMove = !IGridMovable::Execute_PerformAdditionalMovementChecks(Owner, GridPosition, !Ret.bCanMove);
	}
	
	return Ret;
}

bool UGridBasedMovementComponent::CanMoveBetweenMaps() const {
	auto Owner = GetOwner<APawn>();
	return Owner != nullptr && Owner->IsPlayerControlled();
}

void UGridBasedMovementComponent::FaceDirection(EFacingDirection FacingDirection) {
	Direction = FacingDirection;
}

void UGridBasedMovementComponent::WarpToLocation(int32 X, int32 Y, FVector Offset) {
	CurrentPosition = DesiredPosition = {
		FMath::FloorToInt32(Offset.X / UGridUtils::GetGridSize()) + X,
		FMath::FloorToInt32(Offset.Y / UGridUtils::GetGridSize()) + Y
	};
	Offset.X += X * UGridUtils::GetGridSize();
	Offset.Y += Y * UGridUtils::GetGridSize();
	GetOwner()->SetActorLocation(Offset);
}

void UGridBasedMovementComponent::OnMapChanged(IMapGrid& NewMap) const {
	auto Owner = GetOwner<APawn>();
	GUARD(Owner == nullptr || !Owner->IsPlayerControlled(), )
	
	NewMap.OnPlayerEnter();
}

FIntVector2 UGridBasedMovementComponent::GetCurrentPosition() const {
	return CurrentPosition;
}

FIntVector2 UGridBasedMovementComponent::GetDesiredPosition() const {
	return DesiredPosition;
}

EFacingDirection UGridBasedMovementComponent::GetDirection() const {
	return Direction;
}

TArray<FOverlapResult> UGridBasedMovementComponent::HitTestOnFacingTile(EFacingDirection MovementDirection) const {
	static const auto FloatGridSize = static_cast<float>(UGridUtils::GetGridSize());

	FVector LocalOffset(0, 0, 0);
	UGridUtils::AdjustMovementPosition(MovementDirection, LocalOffset);

	auto Owner = GetOwner();
	auto Position = Owner->GetActorLocation();
	auto GridPosition = LocalOffset * UGridUtils::GetGridSize() + Position;
	
	FCollisionShape GridSquare;
	GridSquare.SetBox(FVector3f(FloatGridSize / 4 - 2, FloatGridSize / 4 - 2, FloatGridSize / 4 - 2));
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	TArray<FOverlapResult> Result;
	GetWorld()->OverlapMultiByChannel(Result, GridPosition, Owner->GetActorRotation().Quaternion(),
	                                  ECC_Pawn, GridSquare, Params);

	return Result;
}

void UGridBasedMovementComponent::HitInteraction(const TArray<TScriptInterface<IInteractable>>& Interactables) {
	auto Owner = GetOwner<APawn>();
	GUARD(Owner == nullptr || !Owner->IsPlayerControlled(), )

	for (auto &Interactable : Interactables) {
		if ((Interactable->GetInteractionTypes() & static_cast<uint8>(EInteractionType::Hit)) == 0)
			continue;
		IInteractable::Execute_OnInteract(Interactable.GetObject(), GetOwner(), EInteractionType::Hit);
	}
}

void UGridBasedMovementComponent::UpdateMovement(float DeltaTime) {
	if (!MoveTimer.IsSet())
		return;

	auto Pos = CurrentPosition;
	float& Timer = MoveTimer.GetValue();
	Timer += DeltaTime;

	double MoveSpeed = 0.25;

	auto Owner = GetOwner();
	auto Position = Owner->GetActorLocation();
	if (CurrentPosition.X != DesiredPosition.X) {
		int32 Distance = FMath::Abs(CurrentPosition.X - DesiredPosition.X);
		Position.X = UMathUtilities::LinearInterpolation(CurrentPosition.X * UGridUtils::GetGridSize(),
		                                                 DesiredPosition.X * UGridUtils::GetGridSize(),
		                                                 MoveSpeed * Distance,
		                                                 Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.X = DesiredPosition.X;
		}
	}

	if (CurrentPosition.Y != DesiredPosition.Y) {
		int32 Distance = FMath::Abs(CurrentPosition.Y - DesiredPosition.Y);
		Position.Y = UMathUtilities::LinearInterpolation(CurrentPosition.Y * UGridUtils::GetGridSize(),
		                                                 DesiredPosition.Y * UGridUtils::GetGridSize(),
		                                                 MoveSpeed * Distance,
		                                                 Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.Y = DesiredPosition.Y;
		}
	}

	Owner->SetActorLocation(Position, true);
	if (CurrentPosition != Pos && CurrentPosition == DesiredPosition) {
		MoveComplete();
	}
}

void UGridBasedMovementComponent::UpdateAnimation(float DeltaTime) {
	GridBasedAnimationComponent->UpdateDirection(Direction);

	if (MoveTimer.IsSet() && !GridBasedAnimationComponent->IsMoveAnimationPlaying()) {
		GridBasedAnimationComponent->StartMoveAnimation();
	} else if (StopTimer.IsSet()) {
		auto& Timer = StopTimer.GetValue();
		Timer += DeltaTime;

		if (Timer >= 0.125f && GridBasedAnimationComponent->CanStopMoving()) {
			GridBasedAnimationComponent->StopMoveAnimation();
			StopTimer.Reset();
		}
	}
}

void UGridBasedMovementComponent::MoveComplete() {
	MoveTimer.Reset();
	StopTimer.Emplace(0.f);

	// TODO: Decide if you want to refactor this at all. It might still be helpful to have for one time callbacks.
	if (MoveCallback.IsSet()) {
		auto Callback = MoveTemp(MoveCallback.GetValue());
		MoveCallback.Reset();
		Callback();
	}

	auto MapSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)
	auto Owner = GetOwner();
	ASSERT(Owner != nullptr && Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
	MapSubsystem->UpdateCharacterMapPosition(Owner);
}
