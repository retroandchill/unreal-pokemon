// "Unreal Pokémon" created by Retro & Chill.

#include "Camera/CameraSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Ranges/Optional/IfPresent.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OptionalRef.h"

void UCameraSubsystem::OnWorldBeginPlay(UWorld &InWorld) {
    Super::OnWorldBeginPlay(InWorld);

    // clang-format off
    UE::Optionals::OfNullable(InWorld.GetGameInstance()) |
        UE::Optionals::Map(&UGameInstance::GetPrimaryPlayerController, false) |
        UE::Optionals::Map(&APlayerController::GetLocalPlayer) |
        UE::Optionals::Map(&ULocalPlayer::GetLocalPlayerIndex) |
        UE::Optionals::IfPresent([this, &InWorld](int32 PlayerIndex) {
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

TStatId UCameraSubsystem::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UCameraSubsystem, STATGROUP_Tickables);
}

FOnCameraAngleChanged &UCameraSubsystem::GetOnCameraAngleChanged() {
    return OnCameraAngleChanged;
}

FVector2D UCameraSubsystem::GetCurrentCameraAngle() const {
    return CurrentCameraAngle;
}