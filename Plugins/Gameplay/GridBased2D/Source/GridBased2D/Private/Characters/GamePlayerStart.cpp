// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/GamePlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "GridUtils.h"

// Sets default values
AGamePlayerStart::AGamePlayerStart(const FObjectInitializer &ObjectInitializer) : APlayerStart(ObjectInitializer) {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    float Radius = static_cast<float>(UGridUtils::GetDefaultGridSize()) / 2;
    auto Capsule = GetCapsuleComponent();
    Capsule->SetCapsuleRadius(Radius);
    Capsule->SetCapsuleHalfHeight(Radius);
}
