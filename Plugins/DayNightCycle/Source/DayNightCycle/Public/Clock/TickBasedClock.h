// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameClock.h"
#include "UObject/Object.h"

#include "TickBasedClock.generated.h"

/**
 * Game clock that determines its time based on the number of ticks experienced in the game.
 */
UCLASS(Config = Game)
class DAYNIGHTCYCLE_API UTickBasedClock : public UObject, public IGameClock, public FTickableGameObject {
    GENERATED_BODY()

  public:
    void Tick(float DeltaTime) override;
    TStatId GetStatId() const override;

  protected:
    FDateTime GetCurrentTime_Implementation() const override;

  public:
    /**
     * Set the current time to the new value
     * @param DateTime The new time value to set the clock to
     */
    UFUNCTION(BlueprintCallable, Category = Clock)
    void SetCurrentTime(const FDateTime &DateTime);

  private:
    /**
     * The span of time (in seconds) than an in-game day should last
     */
    UPROPERTY(Config)
    float DayLength = 1440.f;

    /**
     * The current time in-game. The config value represents the starting time when the object is created
     */
    UPROPERTY(Config)
    FDateTime CurrentTime = FDateTime(2000, 1, 1, 8);
};
