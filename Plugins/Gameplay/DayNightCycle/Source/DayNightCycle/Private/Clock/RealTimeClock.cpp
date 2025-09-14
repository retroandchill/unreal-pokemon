// "Unreal Pokémon" created by Retro & Chill.

#include "Clock/RealTimeClock.h"

FDateTime URealTimeClock::GetCurrentTime_Implementation() const
{
    return FDateTime::Now();
}