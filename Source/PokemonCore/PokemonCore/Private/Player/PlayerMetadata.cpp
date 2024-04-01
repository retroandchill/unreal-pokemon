// "Unreal Pokémon" created by Retro & Chill.


#include "Player/PlayerMetadata.h"

#include "Managers/PokemonSubsystem.h"

void UPlayerMetadata::Tick(float DeltaTime) {
	TotalPlaytime += DeltaTime;
}

TStatId UPlayerMetadata::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UPlayerMetadata, STATGROUP_Tickables);
}

UWorld* UPlayerMetadata::GetTickableGameObjectWorld() const {
	return GetWorld();
}

const FDateTime& UPlayerMetadata::GetStartDate() const {
	return StartDate;
}

float UPlayerMetadata::GetTotalPlaytime() const {
	return TotalPlaytime;
}

void UPlayerMetadata::StartNewGame() {
	StartDate = FDateTime::Now();
	TotalPlaytime = 0.f;
}
