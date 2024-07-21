// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameClock.h"
#include "UObject/Object.h"
#include "AcceleratedRealTimeClock.generated.h"

/**
 * Implementation of the real-time clock that condenses the length of the day so that there are multiple in-game
 * days in a real-time day, but the time is still tied to the system clock.
 */
UCLASS(Config = Game)
class DAYNIGHTCYCLE_API UAcceleratedRealTimeClock : public UObject, public IGameClock {
    GENERATED_BODY()

protected:
    FDateTime GetCurrentTime_Implementation() const override;

private:
    /**
     * How many in-game days make up a real day?
     */
    UPROPERTY(Config)
    float InGameDaysPerDay = 20.f;

    /**
     * The start time used to configure the time. This will create the offset for the time at midnight
     */
    UPROPERTY(Config)
    FDateTime InitialTime = FDateTime(2000, 1, 1, 4);

};
