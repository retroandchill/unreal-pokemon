// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GameCharacter.h"

#include "MathUtilities.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"
#include "GridUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGameCharacter::AGameCharacter() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	float BoxSize = static_cast<float>(GridBased2D::GRID_SIZE) / 2;
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
	CurrentPosition.X = FMath::FloorToInt32(Position.X / GridBased2D::GRID_SIZE);
	CurrentPosition.Y = FMath::FloorToInt32(Position.Y / GridBased2D::GRID_SIZE);

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
	if (!CanMoveInDirection(MovementDirection))
		return;

	GridBased2D::AdjustMovementPosition(MovementDirection, DesiredPosition);

	MoveTimer.Emplace(0.f);
	StopTimer.Reset();
}

bool AGameCharacter::CanMoveInDirection(EFacingDirection MovementDirection) const {
	auto Result = HitTestOnFacingTile(MovementDirection);
	return !Result.bBlockingHit;
}

void AGameCharacter::FaceDirection(EFacingDirection FacingDirection) {
	Direction = FacingDirection;
}

FHitResult AGameCharacter::HitTestOnFacingTile(EFacingDirection MovementDirection) const {
	static constexpr auto FloatGridSize = static_cast<float>(GridBased2D::GRID_SIZE);

	FVector LocalOffset(0, 0, 0);
	GridBased2D::AdjustMovementPosition(MovementDirection, LocalOffset);

	auto Position = GetActorLocation();
	auto GridPosition = LocalOffset * GridBased2D::GRID_SIZE + Position;
	FCollisionShape GridSquare;
	GridSquare.SetBox(FVector3f(FloatGridSize / 4 - 2, FloatGridSize / 4 - 2, FloatGridSize / 4 - 2));
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Result;
	GetWorld()->SweepSingleByChannel(Result, Position, GridPosition, GetActorRotation().Quaternion(),
	                                 ECC_Pawn, GridSquare, Params);

	return Result;
}

void AGameCharacter::InitCharacterData() {
	auto Flipbook = GetDesiredFlipbook();
	auto CharacterSprite = GetSprite();
	CharacterSprite->SetFlipbook(Flipbook);
	CharacterSprite->Stop(); // TODO: We want this to toggle depending on the current situation
	CharacterSprite->SetTranslucentSortPriority(static_cast<int32>(GetActorLocation().Y));

	auto Position = GetActorLocation();
	CurrentPosition.X = FMath::FloorToInt32(Position.X / GridBased2D::GRID_SIZE);
	CurrentPosition.Y = FMath::FloorToInt32(Position.Y / GridBased2D::GRID_SIZE);

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

	float& Timer = MoveTimer.GetValue();
	Timer += DeltaTime;

	double MoveSpeed = 0.25;

	auto Position = GetActorLocation();
	if (CurrentPosition.X != DesiredPosition.X) {
		int32 Distance = FMath::Abs(CurrentPosition.X - DesiredPosition.X);
		Position.X = UMathUtilities::LinearInterpolation(CurrentPosition.X * GridBased2D::GRID_SIZE,
		                                                 DesiredPosition.X * GridBased2D::GRID_SIZE,
		                                                 MoveSpeed * Distance,
		                                                 Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.X = DesiredPosition.X;
		}
	}

	if (CurrentPosition.Y != DesiredPosition.Y) {
		int32 Distance = FMath::Abs(CurrentPosition.Y - DesiredPosition.Y);
		Position.Y = UMathUtilities::LinearInterpolation(CurrentPosition.Y * GridBased2D::GRID_SIZE,
		                                                 DesiredPosition.Y * GridBased2D::GRID_SIZE,
		                                                 MoveSpeed * Distance,
		                                                 Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.Y = DesiredPosition.Y;
		}
	}

	SetActorLocation(Position);
	if (CurrentPosition == DesiredPosition) {
		MoveTimer.Reset();
		StopTimer.Emplace(0.f);
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
