// "Unreal Pokémon" created by Retro & Chill.


#include "Map/WarpingUtilities.h"

#include "Asserts.h"
#include "Kismet/GameplayStatics.h"
#include "Map/MapSubsystem.h"
#include "Map/WarpDestination.h"

void UWarpingUtilities::WarpToMap(const UObject* WorldContext, const FWarpDestination& Destination) {
	ASSERT(WorldContext != nullptr)
	auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
	GUARD_WARN(MapSubsystem == nullptr, , TEXT("Failed to get the Map Subsystem!"))
	
	if (Destination.bOverride_Direction) {
		MapSubsystem->WarpToMapWithDirection(Destination.Map, Destination.X, Destination.Y, Destination.Direction);
	} else {
		MapSubsystem->WarpToMap(Destination.Map, Destination.X, Destination.Y);
	}
}
