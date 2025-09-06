// "Unreal Pokémon" created by Retro & Chill.

#include "Camera/CameraUtilities.h"

FRotator UCameraUtilities::TranslateCameraRotation(const FVector2D &Rotation)
{
    return FRotator(0, Rotation.X, Rotation.Y);
}