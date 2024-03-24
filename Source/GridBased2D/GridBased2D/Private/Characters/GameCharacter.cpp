// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GameCharacter.h"

#include "Asserts.h"
#include "MathUtilities.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"
#include "GridUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/Interactable.h"
#include "Map/GridBasedMap.h"
#include "Map/MapSubsystem.h"

// Sets default values
AGameCharacter::AGameCharacter() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	float BoxSize = static_cast<float>(UGridUtils::GRID_SIZE) / 2;
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleRadius(BoxSize);
	Capsule->SetCapsuleHalfHeight(BoxSize);

	auto CharacterSprite = GetSprite();
	CharacterSprite->SetRelativeLocation(FVector(0, BoxSize, 0));
	CharacterSprite->SetRelativeRotation(FRotator(0, 0, -90));

	GetCharacterMovement()->GravityScale = 0;

	if (ConstructorHelpers::FObjectFinder<UMaterialInterface> SpriteMaterial(
		TEXT("/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial")); SpriteMaterial.Succeeded()) {
		GetSprite()->SetMaterial(0, SpriteMaterial.Object);
	}
}

void AGameCharacter::PostInitProperties() {
	Super::PostInitProperties();
	InitCharacterData();
}

void AGameCharacter::PostReinitProperties() {
	Super::PostReinitProperties();
	InitCharacterData();
}

void AGameCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	InitCharacterData();
}

void AGameCharacter::PostLoad() {
	Super::PostLoad();
	InitCharacterData();
}

void AGameCharacter::PostEditMove(bool bFinished) {
	Super::PostEditMove(bFinished);
	InitCharacterData();
}


// Called when the game starts or when spawned
void AGameCharacter::BeginPlay() {
	Super::BeginPlay();
	auto Position = GetActorLocation();
	CurrentPosition.X = FMath::FloorToInt32(Position.X / UGridUtils::GRID_SIZE);
	CurrentPosition.Y = FMath::FloorToInt32(Position.Y / UGridUtils::GRID_SIZE);

	DesiredPosition = CurrentPosition;

	auto CharacterSprite = GetSprite();
	CharacterSprite->Stop();
	CharacterSprite->SetPlaybackPositionInFrames(0, false);
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateMovement(DeltaTime);
	UpdateAnimation(DeltaTime);
}

void AGameCharacter::MoveInDirection(EFacingDirection MovementDirection) {
	FaceDirection(MovementDirection);
	if (auto [bCanMove, FoundActors] = MovementCheck(MovementDirection); !bCanMove) {
		HitInteraction(FoundActors);
		return;
	}

	UGridUtils::AdjustMovementPosition(MovementDirection, DesiredPosition);

	MoveTimer.Emplace(0.f);
	StopTimer.Reset();
}

void AGameCharacter::MoveInDirection(EFacingDirection MovementDirection,
	TFunction<void()> &&MovementCompleteCallback) {
	GUARD_WARN(MoveCallback.IsSet(), , TEXT("The movement timer is already set for character: %s"), *GetName())

	MoveCallback.Emplace(MoveTemp(MovementCompleteCallback));
	MoveInDirection(MovementDirection);
}

FMoveCheckResult AGameCharacter::MovementCheck(EFacingDirection MovementDirection) const {
	FMoveCheckResult Ret;
	auto Maps = UGridUtils::FindAllActors<AGridBasedMap>(this);
	auto DestinationPosition = GetCurrentPosition();
	UGridUtils::AdjustMovementPosition(MovementDirection, DestinationPosition);
	bool bMapFound = false;
	for (auto Map : Maps) {
		if (Map->IsPositionInMap(DestinationPosition)) {
			bMapFound = Map->IsObjectInMap(this) || CanMoveBetweenMaps();
			break;
		}
	}

	if (!bMapFound) {
		Ret.bCanMove = false;
		return Ret;
	}

	for (auto Results = HitTestOnFacingTile(MovementDirection); auto &Result : Results) {
		if (Result.bBlockingHit) {
			Ret.bCanMove = false;
		}

		if (auto Interactable = Cast<IInteractable>(Result.GetActor()); Interactable != nullptr) {
			Ret.FoundActors.Emplace(Result.GetActor());
		}
	}
	return Ret;
}

bool AGameCharacter::CanMoveBetweenMaps() const {
	return false;
}

void AGameCharacter::FaceDirection(EFacingDirection FacingDirection) {
	Direction = FacingDirection;
}

void AGameCharacter::WarpToLocation(int32 X, int32 Y, FVector Offset) {
	CurrentPosition = DesiredPosition = {FMath::FloorToInt32(Offset.X / UGridUtils::GRID_SIZE) + X,
		FMath::FloorToInt32(Offset.Y / UGridUtils::GRID_SIZE) + Y};
	Offset.X += X * UGridUtils::GRID_SIZE;
	Offset.Y += Y * UGridUtils::GRID_SIZE;
	SetActorLocation(Offset);
}

TArray<FOverlapResult> AGameCharacter::HitTestOnFacingTile(EFacingDirection MovementDirection) const {
	static constexpr auto FloatGridSize = static_cast<float>(UGridUtils::GRID_SIZE);

	FVector LocalOffset(0, 0, 0);
	UGridUtils::AdjustMovementPosition(MovementDirection, LocalOffset);

	auto Position = GetActorLocation();
	auto GridPosition = LocalOffset * UGridUtils::GRID_SIZE + Position;
	FCollisionShape GridSquare;
	GridSquare.SetBox(FVector3f(FloatGridSize / 4 - 2, FloatGridSize / 4 - 2, FloatGridSize / 4 - 2));
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<FOverlapResult> Result;
	GetWorld()->OverlapMultiByChannel(Result, GridPosition, GetActorRotation().Quaternion(),
	                                 ECC_Pawn, GridSquare, Params);

	return Result;
}

void AGameCharacter::HitInteraction(const TArray<TScriptInterface<IInteractable>>& Interactables) {
	// No implementation in this class, only the player needs this
}

void AGameCharacter::InitCharacterData() {
	auto Flipbook = GetDesiredFlipbook();
	auto CharacterSprite = GetSprite();
	CharacterSprite->SetFlipbook(Flipbook);
	CharacterSprite->Stop(); // TODO: We want this to toggle depending on the current situation
	CharacterSprite->SetTranslucentSortPriority(static_cast<int32>(GetActorLocation().Y));

	auto Position = GetActorLocation();
	CurrentPosition.X = FMath::FloorToInt32(Position.X / UGridUtils::GRID_SIZE);
	CurrentPosition.Y = FMath::FloorToInt32(Position.Y / UGridUtils::GRID_SIZE);

	DesiredPosition = CurrentPosition;

	Position.Z = 0.0;
	SetActorLocation(Position);
}

UPaperFlipbook* AGameCharacter::GetDesiredFlipbook() const {
	if (Charset == nullptr)
		return nullptr;

	return Charset->GetSprite(Direction);
}

void AGameCharacter::UpdateMovement(float DeltaTime) {
	if (!MoveTimer.IsSet())
		return;

	auto Pos = CurrentPosition;
	float& Timer = MoveTimer.GetValue();
	Timer += DeltaTime;

	double MoveSpeed = 0.25;

	auto Position = GetActorLocation();
	if (CurrentPosition.X != DesiredPosition.X) {
		int32 Distance = FMath::Abs(CurrentPosition.X - DesiredPosition.X);
		Position.X = UMathUtilities::LinearInterpolation(CurrentPosition.X * UGridUtils::GRID_SIZE,
		                                                 DesiredPosition.X * UGridUtils::GRID_SIZE,
		                                                 MoveSpeed * Distance,
		                                                 Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.X = DesiredPosition.X;
		}
	}

	if (CurrentPosition.Y != DesiredPosition.Y) {
		int32 Distance = FMath::Abs(CurrentPosition.Y - DesiredPosition.Y);
		Position.Y = UMathUtilities::LinearInterpolation(CurrentPosition.Y * UGridUtils::GRID_SIZE,
		                                                 DesiredPosition.Y * UGridUtils::GRID_SIZE,
		                                                 MoveSpeed * Distance,
		                                                 Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.Y = DesiredPosition.Y;
		}
	}

	SetActorLocation(Position);
	if (CurrentPosition != Pos && CurrentPosition == DesiredPosition) {
		OnMoveComplete();
	}
}

void AGameCharacter::UpdateAnimation(float DeltaTime) {
	auto Flipbook = GetDesiredFlipbook();
	auto CharacterSprite = GetSprite();
	CharacterSprite->SetFlipbook(Flipbook);

	if (MoveTimer.IsSet() && !CharacterSprite->IsPlaying()) {
		CharacterSprite->PlayFromStart();
		CharacterSprite->SetLooping(true);
	} else if (StopTimer.IsSet()) {
		auto& Timer = StopTimer.GetValue();
		Timer += DeltaTime;

		if (Timer >= 0.125f && Charset->CanStopOnFrame(Direction, CharacterSprite->GetPlaybackPositionInFrames())) {
			CharacterSprite->Stop();
			CharacterSprite->SetPlaybackPositionInFrames(0, false);
			StopTimer.Reset();
		}
	}

	CharacterSprite->SetTranslucentSortPriority(static_cast<int32>(GetActorLocation().Y));
}

void AGameCharacter::OnMoveComplete() {
	MoveTimer.Reset();
	StopTimer.Emplace(0.f);
	
	if (MoveCallback.IsSet()) {
		auto Callback = MoveTemp(MoveCallback.GetValue());
		MoveCallback.Reset();
		Callback();
	}

	auto MapSubsystem = GetGameInstance()->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)
	MapSubsystem->UpdateCharacterMapPosition(this);
}

void AGameCharacter::OnMapChanged(AGridBasedMap* NewMap) {
	// No implementation here for now
}

void AGameCharacter::SetCharset(UCharset* NewCharset) {
	Charset = NewCharset;
}

FIntVector2 AGameCharacter::GetCurrentPosition() const {
	return CurrentPosition;
}

FIntVector2 AGameCharacter::GetDesiredPosition() const {
	return DesiredPosition;
}

EFacingDirection AGameCharacter::GetDirection() const {
	return Direction;
}
