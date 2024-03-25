// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GamePlayerStart.h"

#include "GridUtils.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AGamePlayerStart::AGamePlayerStart(const FObjectInitializer& ObjectInitializer) : APlayerStart(ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	float Radius = UGridUtils::GetGridSize() / 2;
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleRadius(Radius);
	Capsule->SetCapsuleHalfHeight(Radius);
}
