// "Unreal Pokémon" created by Retro & Chill.

#include "Player/PlayerMetadata.h"
#include "RetroLib/Utils/BlueprintMathUtils.h"
#include "RetroLib/Utils/Math.h"

void UPlayerMetadata::Tick(float DeltaTime)
{
    TotalPlaytime += DeltaTime;
    LastUpdated += DeltaTime;

    if (LastUpdated >= Retro::SECONDS_PER_MINUTE)
    {
        OnTimeUpdated.Broadcast(TotalPlaytime);
        LastUpdated = FMath::Fmod(LastUpdated, Retro::SECONDS_PER_MINUTE);
    }
}

TStatId UPlayerMetadata::GetStatId() const {RETURN_QUICK_DECLARE_CYCLE_STAT(UPlayerMetadata, STATGROUP_Tickables)}

UWorld *UPlayerMetadata::GetTickableGameObjectWorld() const
{
    return GetWorld();
}

const FDateTime &UPlayerMetadata::GetStartDate() const
{
    return StartDate;
}

float UPlayerMetadata::GetTotalPlaytime() const
{
    return TotalPlaytime;
}

int32 UPlayerMetadata::GetRepelSteps() const
{
    return RepelSteps;
}

void UPlayerMetadata::SetRepelSteps(int32 NewRepelSteps)
{
    RepelSteps = FMath::Max(0, NewRepelSteps);
}

FOnTimeUpdate &UPlayerMetadata::GetOnTimeUpdated()
{
    return OnTimeUpdated;
}

void UPlayerMetadata::StartNewGame()
{
    StartDate = FDateTime::Now();
    TotalPlaytime = 0.f;
}
