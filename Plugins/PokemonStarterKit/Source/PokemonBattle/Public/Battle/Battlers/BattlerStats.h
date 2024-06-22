// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattlerStats.generated.h"

/**
 * Represents a main battle stat in the game
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FMainBattleStat {
    GENERATED_BODY()

    /**
     * The unaltered numerical value of the stat
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats)
    int32 StatValue;

    /**
     * The number of stages the stat has been affected by
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats)
    int32 Stages;

    /**
     * Get the modified value of the stat
     * @return The modified value of the stat
     */
    int32 GetModifiedValue() const;
};