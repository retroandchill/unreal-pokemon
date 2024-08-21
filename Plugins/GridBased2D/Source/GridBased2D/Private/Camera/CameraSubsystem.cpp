// "Unreal Pokémon" created by Retro & Chill.

#include "Camera/CameraSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCameraSubsystem::OnWorldBeginPlay(UWorld &InWorld) {
    Super::OnWorldBeginPlay(InWorld);

    auto PlayerIndex = InWorld.GetGameInstance()->GetPrimaryPlayerController(false)
        ->GetLocalPlayer()->GetLocalPlayerIndex();
    PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(&InWorld, PlayerIndex);
}

void UCameraSubsystem::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    auto CameraRotation = PlayerCameraManager->GetCameraRotation();
    if (FVector2D NewRotation(CameraRotation.Yaw + ZRotationAdjustment, CameraRotation.Pitch);
            !CurrentCameraAngle.Equals(NewRotation)) {
        SetCurrentCameraAngle(NewRotation);
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

void UCameraSubsystem::SetCurrentCameraAngle(FVector2D NewAngle) {
    CurrentCameraAngle = NewAngle;
    OnCameraAngleChanged.Broadcast(NewAngle);
}