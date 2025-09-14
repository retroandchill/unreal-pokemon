// "Unreal Pokémon" created by Retro & Chill.

#include "Time/TimeBlueprintLibrary.h"

FDateTime UTimeBlueprintLibrary::GetDateTimeNow()
{
    return FDateTime::Now();
}