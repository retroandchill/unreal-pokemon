// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * RAII container that creates a temporary RNG seed and resets it upon deletion
 */
class AUTOMATIONTESTHELPERS_API FTemporarySeed {
public:
    /**
     * Initialize the RNG with a new fixed seed
     * @param Seed The new random seed
     */
    explicit FTemporarySeed(int32 Seed);

    /**
     * Destructor to re-randomize the seed using the current time
     */
    ~FTemporarySeed();
};
