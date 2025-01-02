// "Unreal Pokémon" created by Retro & Chill.

#include "Camera/CameraSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "RetroLib.h"

void UCameraSubsystem::OnWorldBeginPlay(UWorld &InWorld) {
    Super::OnWorldBeginPlay(InWorld);

    // clang-format off
    Retro::Optionals::OfNullable(InWorld.GetGameInstance()) |
        Retro::Optionals::Transform<&UGameInstance::GetPrimaryPlayerController>(false) |
        Retro::Optionals::Transform<&APlayerController::GetLocalPlayer>() |
        Retro::Optionals::Transform<&ULocalPlayer::GetLocalPlayerIndex>() |
        Retro::Optionals::IfPresent([this, &InWorld](int32 PlayerIndex) {
            PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(&InWorld, PlayerIndex);
        });
    // clang-format on
}

void UCameraSubsystem::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    auto CameraRotation = PlayerCameraManager->GetCameraRotation();
    if (FVector2D NewRotation(CameraRotation.Yaw + ZRotationAdjustment, CameraRotation.Pitch);
        !CurrentCameraAngle.Equals(NewRotation)) {
        CurrentCameraAngle = NewRotation;
        OnCameraAngleChanged.Broadcast(CurrentCameraAngle);
    }
}

TStatId UCameraSubsystem::GetStatId() const {RETURN_QUICK_DECLARE_CYCLE_STAT(UCameraSubsystem, STATGROUP_Tickables)}

FDelegateHandle UCameraSubsystem::BindToGetOnCameraAngleChanged(FOnCameraAngleChanged::FDelegate &&Binding) {
    return OnCameraAngleChanged.Add(std::move(Binding));
}

FVector2D UCameraSubsystem::GetCurrentCameraAngle() const {
    return CurrentCameraAngle;
}