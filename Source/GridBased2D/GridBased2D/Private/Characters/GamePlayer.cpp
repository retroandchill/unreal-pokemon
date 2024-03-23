// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GamePlayer.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/Charset.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridUtils.h"
#include "Actions/Action.h"
#include "Interaction/Interactable.h"
#include "Map/MapSubsystem.h"


// Sets default values
AGamePlayer::AGamePlayer() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, 0.0f, -90.0f));


	// Create an orthographic camera (no perspective) and attach it to the boom
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	TopDownCamera->OrthoWidth = 512.f;
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	TopDownCamera->bUsePawnControlRotation = false;
	TopDownCamera->bAutoActivate = true;
}

// Called when the game starts or when spawned
void AGamePlayer::BeginPlay() {
	Super::BeginPlay();

	if (auto MapSubsystem = GetGameInstance()->GetSubsystem<UMapSubsystem>(); MapSubsystem != nullptr) {
		MapSubsystem->SetPlayerLocation(this);
	}
	
	if (const auto* const PlayerController = Cast<APlayerController>(Controller); PlayerController != nullptr) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


// Called to bind functionality to input
void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	using enum ETriggerEvent;

	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveInput.Get(), Triggered, this, &AGamePlayer::Move);
	Input->BindAction(FaceDirectionInput.Get(), Triggered, this, &AGamePlayer::Turn);
	Input->BindAction(InteractInput.Get(), Triggered, this, &AGamePlayer::Interact);
	Input->BindAction(PauseInput.Get(), Triggered, this, &AGamePlayer::PauseGame);
}

void AGamePlayer::HitInteraction(const TArray<TScriptInterface<IInteractable>>& Interactables) {
	Super::HitInteraction(Interactables);
	for (auto &Interactable : Interactables) {
		if ((Interactable->GetInteractionTypes() & static_cast<uint8>(EInteractionType::Hit)) == 0)
			continue;
		IInteractable::Execute_OnInteract(Interactable.GetObject(), this, EInteractionType::Hit);
	}
}

void AGamePlayer::Move(const FInputActionInstance& Input) {
	auto Vector = Input.GetValue().Get<FVector2D>();
	auto Dir = GridBased2D::VectorToFacingDirection(Vector);
	if (!Dir.IsSet() || GetCurrentPosition() != GetDesiredPosition())
		return;

	MoveInDirection(Dir.GetValue());
}

void AGamePlayer::Turn(const FInputActionInstance& Input) {
	auto Vector = Input.GetValue().Get<FVector2D>();
	auto Dir = GridBased2D::VectorToFacingDirection(Vector);
	if (!Dir.IsSet() || GetCurrentPosition() != GetDesiredPosition())
		return;

	FaceDirection(Dir.GetValue());
}

void AGamePlayer::Interact() {
	for (auto Results = HitTestOnFacingTile(GetDirection()); auto &Result : Results) {
		if (auto Interactable = Cast<IInteractable>(Result.GetActor()); Interactable == nullptr ||
			(Interactable->GetInteractionTypes() & static_cast<uint8>(EInteractionType::Talk)) == 0)
			continue;
		
		IInteractable::Execute_OnInteract(Result.GetActor(), this, EInteractionType::Talk);
	}
	
}

void AGamePlayer::PauseGame() {
	PauseAction->PerformAction(this);
}
