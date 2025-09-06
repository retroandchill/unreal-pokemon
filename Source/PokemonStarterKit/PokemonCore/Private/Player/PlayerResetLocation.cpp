// "Unreal Pokémon" created by Retro & Chill.

#include "Player/PlayerResetLocation.h"

FPlayerResetLocation::FPlayerResetLocation(FStringView MapName, const FTransform &Transform)
    : MapName(MapName), PlayerTransform(Transform)
{
}

const FString &FPlayerResetLocation::GetMapName() const
{
    return MapName;
}

const FTransform &FPlayerResetLocation::GetPlayerTransform() const
{
    return PlayerTransform;
}