// "Unreal Pokémon" created by Retro & Chill.


#include "Clock/TickBasedClock.h"

constexpr float HOURS_PER_DAY = 24.f;

void UTickBasedClock::Tick(float DeltaTime) {
    CurrentTime += FTimespan::FromHours(DeltaTime / DayLength * HOURS_PER_DAY);
}

TStatId UTickBasedClock::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UTickBasedClock, STATGROUP_Tickables)
}

FDateTime UTickBasedClock::GetCurrentTime_Implementation() const {
    return CurrentTime;
}