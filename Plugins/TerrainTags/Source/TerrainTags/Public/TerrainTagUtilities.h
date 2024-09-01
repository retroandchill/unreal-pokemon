// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TerrainTagUtilities.generated.h"

struct FGameplayTagContainer;
struct FGameplayTag;

/**
 * Utility functions for handling terrain tag related operations.
 */
UCLASS()
class TERRAINTAGS_API UTerrainTagUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Does this actor have the given terrain tag?
     * @param WorldContext The object used to obtain information about the world
     * @param Tag The tag to search for
     * @param Character The origin for the search
     * @return Is the given tag present?
     */
    UFUNCTION(BlueprintPure, Category = TerrainTags,
        meta = (AutoCreateRefTerm = Tag, WorldContext = WorldContext))
    static bool HasTerrainTag(const UObject* WorldContext, const FGameplayTag& Tag, const ACharacter* Character);

    /**
     * Does this actor have any of the given terrain tags?
     * @param WorldContext The object used to obtain information about the world
    * @param Tags The tags to search for
     * @param Character The origin for the search
     * @return Is at least a single terrain tag present?
     */
    UFUNCTION(BlueprintPure, Category = TerrainTags, meta = (AutoCreateRefTerm = Tags))
    static bool HasAnyTerrainTag(const UObject* WorldContext, const FGameplayTagContainer& Tags, const ACharacter* Character);

    /**
     * Does this actor have all of the given terrain tags?
     * @param WorldContext The object used to obtain information about the world
    * @param Tags The tags to search for
     * @param Character The origin for the search
     * @return Are all the terrain tags present?
     */
    UFUNCTION(BlueprintPure, Category = TerrainTags, meta = (AutoCreateRefTerm = Tags))
    static bool HasAllTerrainTags(const UObject* WorldContext, const FGameplayTagContainer& Tags, ACharacter* Character);

};
