// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameClock.h"
#include "UObject/Object.h"

#include "RealTimeClock.generated.h"

/**
 * Game Clock implementation that uses the system time to set the current time.
 */
UCLASS()
class DAYNIGHTCYCLE_API URealTimeClock : public UObject, public IGameClock {
    GENERATED_BODY()

  protected:
    FDateTime GetCurrentTime_Implementation() const override;
};
