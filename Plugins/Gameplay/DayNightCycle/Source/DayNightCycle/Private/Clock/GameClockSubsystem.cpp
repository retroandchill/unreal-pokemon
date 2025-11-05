// "Unreal Pokémon" created by Retro & Chill.

#include "Clock/GameClockSubsystem.h"
#include "DayNightCycleSettings.h"

class UDayNightCycleSettings;

void UGameClockSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    GameClock = NewObject<UObject>(this, GetDefault<UDayNightCycleSettings>()->ClockClass);
}

void UGameClockSubsystem::Deinitialize() {
    Super::Deinitialize();
    GameClock = nullptr;
}

const TScriptInterface<IGameClock> &UGameClockSubsystem::GetGameClock() const {
    return GameClock;
}

bool UGameClockSubsystem::IsDay() const {
    auto CurrentTime = IGameClock::Execute_GetCurrentTime(GameClock.GetObject());
    auto CurrentHour = static_cast<float>(CurrentTime.GetTimeOfDay().GetTotalHours());
    return GetDefault<UDayNightCycleSettings>()->DayRange.Contains(CurrentHour);
}

float UGameClockSubsystem::GetDayCoefficient() const {
    if (!IsDay()) {
        return 0.f;
    }

    auto CurrentTime = IGameClock::Execute_GetCurrentTime(GameClock.GetObject());
    auto CurrentHour = static_cast<float>(CurrentTime.GetTimeOfDay().GetTotalHours());
    auto &DayRange = GetDefault<UDayNightCycleSettings>()->DayRange;
    return (CurrentHour - DayRange.GetLowerBoundValue()) /
           (DayRange.GetUpperBoundValue() - DayRange.GetLowerBoundValue());
}