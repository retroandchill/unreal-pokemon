// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GamePlayerStart.h"

#include "GridUtils.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AGamePlayerStart::AGamePlayerStart(const FObjectInitializer& ObjectInitializer) : APlayerStart(ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	double Radius = GridBased2D::GGridSize / 2;;
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleRadius(Radius);
	Capsule->SetCapsuleHalfHeight(Radius);
}

// Called when the game starts or when spawned
void AGamePlayerStart::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AGamePlayerStart::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

