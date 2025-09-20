// "Unreal Pokémon" created by Retro & Chill.

#include "Camera/CameraSubsystem.h"
#include "OptionalPtr.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "RangeV3.h"

void UCameraSubsystem::OnWorldBeginPlay(UWorld &InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    // clang-format off
    TOptionalPtr(InWorld.GetGameInstance())
        .Map(ranges::bind_back(&UGameInstance::GetPrimaryPlayerController, false))
        .Map(&APlayerController::GetLocalPlayer)
        .IfPresent([this, &InWorld](ULocalPlayer *Player) {
            PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(&InWorld, Player->GetLocalPlayerIndex());
        });
    // clang-format on
}

void UCameraSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    auto CameraRotation = PlayerCameraManager->GetCameraRotation();
    if (FVector2D NewRotation(CameraRotation.Yaw + ZRotationAdjustment, CameraRotation.Pitch);
        !CurrentCameraAngle.Equals(NewRotation))
    {
        CurrentCameraAngle = NewRotation;
        OnCameraAngleChanged.Broadcast(CurrentCameraAngle);
    }
}

TStatId UCameraSubsystem::GetStatId() const {RETURN_QUICK_DECLARE_CYCLE_STAT(UCameraSubsystem, STATGROUP_Tickables)}

FDelegateHandle UCameraSubsystem::BindToGetOnCameraAngleChanged(FOnCameraAngleChanged::FDelegate &&Binding)
{
    return OnCameraAngleChanged.Add(std::move(Binding));
}

FVector2D UCameraSubsystem::GetCurrentCameraAngle() const
{
    return CurrentCameraAngle;
}