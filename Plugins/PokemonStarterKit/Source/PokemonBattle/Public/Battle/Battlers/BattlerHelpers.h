// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utilities/PokemonUtilities.h"

#include "BattlerHelpers.generated.h"

struct FExpGainInfo;
class IBattler;
struct FGameplayAbilityActorInfo;
/**
 * Library of helper functions used for battlers
 */
UCLASS()
class POKEMONBATTLE_API UBattlerHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the value of a battler's stat for the given attribute
     * @param ActorInfo The information about the actor in question
     * @param Attribute The attribute to find
     * @param bFound Was the attribute in question found
     * @return The value of the attribute
     */
    UFUNCTION(BlueprintPure, Category = "Battler|Stats")
    static float GetBattlerStat(const FGameplayAbilityActorInfo &ActorInfo, FGameplayAttribute Attribute, bool &bFound);
};
