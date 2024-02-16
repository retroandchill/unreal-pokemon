//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "Characters/GameCharacter.h"

#include "InputAction.h"
#include "KismetTraceUtils.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"
#include "Components/BoxComponent.h"
#include "..\..\Public\GridUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AGameCharacter::AGameCharacter() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	double BoxSize = GridBased2D::GGridSize / 2;
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleRadius(BoxSize);
	Capsule->SetCapsuleHalfHeight(BoxSize);

	auto CharacterSprite = GetSprite();
	CharacterSprite->SetRelativeLocation(FVector(0, BoxSize, 0));
	CharacterSprite->SetRelativeRotation(FRotator(0, 0, -90));

	GetCharacterMovement()->GravityScale = 0;
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

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay() {
	Super::BeginPlay();
	auto Position = GetActorLocation();
	CurrentPosition.X = FMath::FloorToInt(Position.X / GridBased2D::GGridSize);
	CurrentPosition.Y = FMath::FloorToInt(Position.Y / GridBased2D::GGridSize);

	DesiredPosition = CurrentPosition;
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
	static constexpr auto FloatGridSize = static_cast<float>(GridBased2D::GGridSize);
	
	FVector LocalOffset(0, 0, 0);
	GridBased2D::AdjustMovementPosition(MovementDirection, LocalOffset);

	auto Position = GetActorLocation();
	auto GridPosition = LocalOffset * GridBased2D::GGridSize + Position;
	FHitResult Result;
	FCollisionShape GridSquare;
	GridSquare.SetBox(FVector3f(FloatGridSize / 4 - 2, FloatGridSize / 4 - 2, FloatGridSize / 4 - 2));
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->SweepSingleByChannel(Result, Position, GridPosition, GetActorRotation().Quaternion(),
		ECC_Pawn, GridSquare, Params);

	return !Result.bBlockingHit;
}

void AGameCharacter::FaceDirection(EFacingDirection FacingDirection) {
	Direction = FacingDirection;
}

void AGameCharacter::InitCharacterData() {
	auto Flipbook = GetDesiredFlipbook();
	auto CharacterSprite = GetSprite();
	CharacterSprite->SetFlipbook(Flipbook);
	CharacterSprite->Stop(); // TODO: We want this to toggle depending on the current situation
}

UPaperFlipbook* AGameCharacter::GetDesiredFlipbook() const {
	if (Charset == nullptr)
		return nullptr;

	return Charset->GetSprite(Direction);
}

void AGameCharacter::UpdateMovement(float DeltaTime) {
	if (!MoveTimer.IsSet())
		return;

	float &Timer = MoveTimer.GetValue();
	Timer += DeltaTime;

	double MoveSpeed = 0.25;
	
	auto Position = GetActorLocation();
	if (CurrentPosition.X != DesiredPosition.X) {
		int32 Distance = FMath::Abs(CurrentPosition.X - DesiredPosition.X);
		Position.X = GridBased2D::LinearInterpolation(CurrentPosition.X * GridBased2D::GGridSize,
													  DesiredPosition.X * GridBased2D::GGridSize,
													  MoveSpeed * Distance,
													  Timer);

		if (Timer >= MoveSpeed * Distance) {
			CurrentPosition.X = DesiredPosition.X;
		}
	}

	if (CurrentPosition.Y != DesiredPosition.Y) {
		int32 Distance = FMath::Abs(CurrentPosition.Y - DesiredPosition.Y);
		Position.Y = GridBased2D::LinearInterpolation(CurrentPosition.Y * GridBased2D::GGridSize,
													  DesiredPosition.Y * GridBased2D::GGridSize,
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
		auto &Timer = StopTimer.GetValue();
		Timer += DeltaTime;

		if (Timer >= 0.125f && Charset->CanStopOnFrame(Direction, CharacterSprite->GetPlaybackPositionInFrames())) {
			CharacterSprite->Stop();
			CharacterSprite->SetPlaybackPositionInFrames(0, false);
			StopTimer.Reset();
		}
	}
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
