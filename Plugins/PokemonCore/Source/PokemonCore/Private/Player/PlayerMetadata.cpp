// "Unreal Pokémon" created by Retro & Chill.

#include "Player/PlayerMetadata.h"

#include "MathUtilities.h"

void UPlayerMetadata::Tick(float DeltaTime) {
    TotalPlaytime += DeltaTime;
    LastUpdated += DeltaTime;

    if (LastUpdated >= UMathUtilities::SECONDS_PER_MINUTE) {
        OnTimeUpdated.Broadcast(TotalPlaytime);
        LastUpdated = FMath::Fmod(LastUpdated, UMathUtilities::SECONDS_PER_MINUTE);
    }
}

TStatId UPlayerMetadata::GetStatId() const {RETURN_QUICK_DECLARE_CYCLE_STAT(UPlayerMetadata, STATGROUP_Tickables)}

UWorld *UPlayerMetadata::GetTickableGameObjectWorld() const {
    return GetWorld();
}

const FDateTime &UPlayerMetadata::GetStartDate() const {
    return StartDate;
}

float UPlayerMetadata::GetTotalPlaytime() const {
    return TotalPlaytime;
}

FOnTimeUpdate &UPlayerMetadata::GetOnTimeUpdated() {
    return OnTimeUpdated;
}

void UPlayerMetadata::StartNewGame() {
    StartDate = FDateTime::Now();
    TotalPlaytime = 0.f;
}
