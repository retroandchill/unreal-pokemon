// "Unreal Pokémon" created by Retro & Chill.

#include "Clock/AcceleratedRealTimeClock.h"

FDateTime UAcceleratedRealTimeClock::GetCurrentTime_Implementation() const {
    return InitialTime + FDateTime::Now().GetTimeOfDay() * InGameDaysPerDay;
}