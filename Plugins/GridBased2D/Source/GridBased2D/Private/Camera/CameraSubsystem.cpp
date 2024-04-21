// "Unreal Pokémon" created by Retro & Chill.


#include "Camera/CameraSubsystem.h"

FOnCameraAngleChanged & UCameraSubsystem::GetOnCameraAngleChanged() {
    return OnCameraAngleChanged;
}

FVector2D UCameraSubsystem::GetCurrentCameraAngle() const {
    return CurrentCameraAngle;
}

void UCameraSubsystem::SetCurrentCameraAngle(FVector2D NewAngle) {
    CurrentCameraAngle = NewAngle;
    OnCameraAngleChanged.Broadcast(NewAngle);
}