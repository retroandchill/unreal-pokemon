// "Unreal Pokémon" created by Retro & Chill.

#include "Map/WarpingUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Map/MapSubsystem.h"
#include "Map/WarpDestination.h"

void UWarpingUtilities::WarpToMap(const UObject *WorldContext, const FWarpDestination &Destination) {
    check(WorldContext != nullptr)
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    if (MapSubsystem == nullptr) {
        UE_LOG(LogBlueprint, Warning, TEXT("Failed to get the Map Subsystem!"))
        return;
    }

    if (Destination.bOverride_Direction) {
        MapSubsystem->WarpToMapWithDirection(Destination.Map, Destination.X, Destination.Y, Destination.Direction);
    } else {
        MapSubsystem->WarpToMap(Destination.Map, Destination.X, Destination.Y);
    }
}
