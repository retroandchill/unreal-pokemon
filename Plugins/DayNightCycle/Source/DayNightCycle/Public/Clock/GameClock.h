// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameClock.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UGameClock : public UInterface {
    GENERATED_BODY()
};

/**
 * Abstract definition of the clock used to get the actual in-game time
 */
class DAYNIGHTCYCLE_API IGameClock {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Get the current time to be used by the game.
     * @return The current time in the game
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Clock)
    FDateTime GetCurrentTime() const;
};