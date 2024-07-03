// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Battle/Events/TargetedEvents.h"
#include "Moves/MoveDamageCategory.h"

namespace Pokemon::Battle::Moves {

/**
 * Tag applied to a battler while they're using a move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove)

/**
 * Tag for the event fired when the move's type is determined
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TypeDetermined)

/**
 * Tag applied to a battler while using a Physical move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove_Physical)

/**
 * Tag applied to a battler while using a Special move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove_Special)

/**
 * Tag applied to a battler while using a Status move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UsingMove_Status)

/**
 * Tag applied to the target of a move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget)

/**
 * Tag applied to a battler when they are unaffected by said move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected)

/**
 * Tag applied to a battler when the move missed
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected_Missed)

/**
 * Tag applied to a battler when a move fails
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected_Failed)

/**
 * Tag applied to a battler when a move has no effect due to a type-match up
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Unaffected_NoEffect)

/**
 * Tag applied to a battler when they should take no damage from a move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_NoDamage)

/**
 * Tag applied to a battler when the move scores a critical hit
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_CriticalHit)

/**
 * Tag applied to a battler who is being targeted by a Physical move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Physical)

/**
 * Tag applied to a battler who is being targeted by a Special move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Special)

/**
 * Tag applied to a battler who is being targeted by a Status move
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveTarget_Status)

/**
 * Event tags send out when checking success against a target
 */
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(SuccessCheckAgainstTarget)

/**
 * Event tags send out when performing an accuracy check against a target
 */
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(HitCheckEvents)

/**
 * Event tags send out when performing a critical hit check against a target
 */
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(CriticalHitRateModEvents)

/**
 * Event tags used when performing damage calculation to perform additional modifications to the damage amount
 */
POKEMONBATTLE_API DECLARE_TARGETED_EVENT_TYPE(DamageModificationEvents)

/**
 * Tag used to set the damage amount when damage is being dealt to a battler
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageDealtValue)

/**
 * Tag applied to a battler when they are using a two-turn attack
 */
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TwoTurnAttack)

/**
 * Get the tag for the user of a move for the given damage category
 * @param Category The category of the move
 * @return The tag to use
 */
POKEMONBATTLE_API const FNativeGameplayTag &GetUserCategoryTag(EMoveDamageCategory Category);

/**
 * Get the tag for the target of a move for the given damage category
 * @param Category The category of the move
 * @return The tag to use
 */
POKEMONBATTLE_API const FNativeGameplayTag &GetTargetCategoryTag(EMoveDamageCategory Category);

/**
 * The format used for the traits of various moves that are indicated by their tags
 */
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