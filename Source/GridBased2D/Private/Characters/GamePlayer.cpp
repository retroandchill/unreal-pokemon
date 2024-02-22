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
#include "Characters/GamePlayer.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/Charset.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridUtils.h"


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

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AGamePlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveInput.Get(), ETriggerEvent::Triggered, this, &AGamePlayer::Move);
	Input->BindAction(FaceDirectionInput.Get(), ETriggerEvent::Triggered, this, &AGamePlayer::Turn);
	Input->BindAction(PauseInput.Get(), ETriggerEvent::Triggered, this, &AGamePlayer::PauseGame);
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

