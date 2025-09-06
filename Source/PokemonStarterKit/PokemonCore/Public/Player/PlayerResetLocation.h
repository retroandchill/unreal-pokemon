// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Structure that contains information about the player's location when they reset after a defeat.
 */
class POKEMONCORE_API FPlayerResetLocation
{
  public:
    /**
     * Create a new reset location from the given player character actor.
     * @param MapName The name of the map the player should travel to on a reset.
     * @param Transform The transform that the player should reset to.
     */
    explicit FPlayerResetLocation(FStringView MapName, const FTransform &Transform);

    /**
     * Get the name of the map the player should travel to on a reset.
     * @return The name of the map the player should travel to on a reset.
     */
    const FString &GetMapName() const;

    /**
     * Get the transform that the player should reset to.
     * @return The transform that the player should reset to.
     */
    const FTransform &GetPlayerTransform() const;

  private:
    FString MapName;
    FTransform PlayerTransform;
};
