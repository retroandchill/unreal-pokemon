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

const TScriptInterface<IGameClock> & UGameClockSubsystem::GetGameClock() const {
    return GameClock;
}