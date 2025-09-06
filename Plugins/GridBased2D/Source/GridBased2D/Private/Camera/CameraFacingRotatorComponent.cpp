// "Unreal Pokémon" created by Retro & Chill.

#include "Camera/CameraFacingRotatorComponent.h"
#include "Camera/CameraSubsystem.h"
#include "Camera/CameraUtilities.h"

using FRotateEvent = FOnCameraAngleChanged::FDelegate;

UCameraFacingRotatorComponent::UCameraFacingRotatorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCameraFacingRotatorComponent::BeginPlay()
{
    Super::BeginPlay();

    auto CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>();
    check(IsValid(CameraSubsystem))
    CameraSubsystem->BindToGetOnCameraAngleChanged(
        FRotateEvent::CreateUObject(this, &UCameraFacingRotatorComponent::AdjustCameraRotation));
    AdjustCameraRotation(CameraSubsystem->GetCurrentCameraAngle());
}

void UCameraFacingRotatorComponent::AdjustCameraRotation(const FVector2D &Angle) const
{
#if WITH_EDITOR
    if (!IsValid(RotatedComponent))
    {
        return;
    }
#endif
    RotatedComponent->SetWorldRotation(UCameraUtilities::TranslateCameraRotation(Angle));
    OnComponentRotated.Broadcast(Angle);
}
