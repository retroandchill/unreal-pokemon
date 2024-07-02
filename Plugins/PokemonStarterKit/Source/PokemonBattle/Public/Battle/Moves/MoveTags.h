// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Battle/Events/TargetedEvents.h"
#include "Moves/MoveDamageCategory.h"

namespace Pokemon::Battle::Moves {

POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TypeDetermined)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove_Physical)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove_Special)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove_Status)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected_Missed)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected_Failed)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_NoDamage)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_CriticalHit)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Physical)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Special)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Status)
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(SuccessCheckAgainstTarget)
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(HitCheckEvents)
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(CriticalHitRateModEvents)
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(DamageModificationEvents)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageDealtValue)

POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TwoTurnAttack)

POKEMONBATTLE_API const FNativeGameplayTag &GetUserCategoryTag(EMoveDamageCategory Category);
POKEMONBATTLE_API const FNativeGameplayTag &GetTargetCategoryTag(EMoveDamageCategory Category);

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