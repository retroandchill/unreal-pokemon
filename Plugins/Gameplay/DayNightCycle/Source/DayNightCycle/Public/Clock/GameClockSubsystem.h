// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GameClockSubsystem.generated.h"

class IGameClock;

/**
 * Subsystem used for managing the game's clock and emitting events related to the time change.
 */
UCLASS()
class DAYNIGHTCYCLE_API UGameClockSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;

    /**
     * Get the clock interface object
     * @return The clock object used to handle the current time
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Clock)
    const TScriptInterface<IGameClock> &GetGameClock() const;

    /**
     * Check if the current time is presently daytime
     * @return Is the current time in the day?
     */
    UFUNCTION(BlueprintPure, Category = Clock)
    bool IsDay() const;

    /**
     * Get the coefficient for the day span, which can be used to lerp the position of the sun as needed
     * @return The coefficient of hour far into the day it is (or 0 if it is night)
     */
    UFUNCTION(BlueprintPure, Category = Clock)
    float GetDayCoefficient() const;

  private:
    /**
     * The clock object used to handle the current time
     */
    UPROPERTY(BlueprintGetter = GetGameClock, Category = Clock)
    TScriptInterface<IGameClock> GameClock;
};
