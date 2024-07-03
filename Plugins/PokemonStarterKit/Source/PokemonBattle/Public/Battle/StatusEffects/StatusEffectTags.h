// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle::StatusEffects {

/**
 * The format used for the status effect tags
 */
constexpr auto StatusEffectTagFormat = TEXT("Battle.StatusEffects.{0}");

/**
 * Singleton class for looking up status effect tags by their ID
 */
class POKEMONBATTLE_API FLookup {
    FLookup();
    ~FLookup();

public:
    /**
     * Get the singleton instance of this class
     * @return The sole instance of the class
     */
    static FLookup& GetInstance();

    FORCEINLINE const FNativeGameplayTag& GetTag(FName ID) {
        return *Tags.FindChecked(ID);
    }

private:
    /**
     * The map of status effect IDs to native gameplay tags.
     */
    TMap<FName, TSharedRef<FNativeGameplayTag>> Tags;
};

}
