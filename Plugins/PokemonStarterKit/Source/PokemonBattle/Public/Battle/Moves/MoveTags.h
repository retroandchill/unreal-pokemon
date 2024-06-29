// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Battle/Events/TargetedEvents.h"

namespace Pokemon::Battle::Moves {

POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove)
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(SuccessCheckAgainstTarget)

POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TwoTurnAttack)

constexpr auto MoveTagFormat = TEXT("Battle.Moves.Traits.{0}");

/**
 * Singleton object that handles converting user-defined move tags into GameplayTags
 */
class POKEMONBATTLE_API FLookup {
    FLookup();
    ~FLookup();

public:
    /**
     * Get the singleton instance
     * @return The single instance of this move
     */
    static FLookup& GetInstance();

    /**
     * Get the tag that's associated with the given tag name
     * @param TagName The name of tag to search for
     * @return The found tag
     */
    FORCEINLINE const FNativeGameplayTag &GetTag(FName TagName) const {
        return *Tags.FindChecked(TagName);
    }

private:
    /**
     * The list of tags held by this object
     */
    TMap<FName, TSharedRef<FNativeGameplayTag>> Tags;
    
};

}