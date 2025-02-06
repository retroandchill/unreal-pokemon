// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Clock/RealTimeClock.h"
#include "Templates/SubclassOf.h"
#include "Engine/DeveloperSettings.h"

#include "DayNightCycleSettings.generated.h"

/**
 * Settings related to the day/night cycle
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Day/Night Cycle"))
class DAYNIGHTCYCLE_API UDayNightCycleSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The class used to handle the clock object
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Clock, meta = (MustImplement = "GameClock"))
    TSubclassOf<UObject> ClockClass = URealTimeClock::StaticClass();

    /**
     * The span of time (in hours on a 24-hour clock) that is used to determine the day.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Clock,
              meta = (UIMin = 0.f, UIMax = 24.f, ClampMin = 0.f, ClampMax = 24.f))
    FFloatRange DayRange = FFloatRange(4.f, 20.f);
};
