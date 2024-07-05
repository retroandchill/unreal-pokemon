// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle::Stats {

/**
 * The tag format for ignoring positive stat stages
 */
constexpr auto IgnorePositiveStatsFormat = TEXT("Battle.Battler.Stats.Stages.Ignore.Positive.{0}");

/**
 * The tag format for ignoring negative stat stages
 */
constexpr auto IgnoreNegativeStatsFormat = TEXT("Battle.Battler.Stats.Stages.Ignore.Negative.{0}");

/**
 * Singleton class that contains the various tags used for stat calculation
 */
class POKEMONBATTLE_API FLookup {
    FLookup();
    ~FLookup();

public:
    /**
     * Get the singleton instance of this class
     * @return The sole instance of this class
     */
    static FLookup &Get();

    /**
     * Get the tag for ignoring a positive stat change
     * @param Stat The stat to look up the value for
     * @return The tag for ignoring a positive stat change
     */
    FORCEINLINE FNativeGameplayTag& GetIgnorePositiveTag(FName Stat) const {
        return *IgnorePositiveTags.FindChecked(Stat);
    }

    /**
     * Get the tag for ignoring a negative stat change
     * @param Stat The stat to look up the value for
     * @return The tag for ignoring a negative stat change
     */
    FORCEINLINE FNativeGameplayTag& GetIgnoreNegativeTag(FName Stat) const {
        return *IgnoreNegativeTags.FindChecked(Stat);
    }

private:
    /**
     * The tags for ignoring positive stat changes
     */
    TMap<FName, TSharedRef<FNativeGameplayTag>> IgnorePositiveTags;
    
    /**
     * The tags for ignoring negative stat changes
     */
    TMap<FName, TSharedRef<FNativeGameplayTag>> IgnoreNegativeTags;
    
};

}