// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "Terrain.generated.h"

struct FGameplayTag;
struct FGameplayTagContainer;

// This class does not need to be modified.
UINTERFACE()
class MAPMETADATA_API UTerrain : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface to an actor type that represents a type of terrain
 */
class MAPMETADATA_API ITerrain {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Does this actor have the given terrain tag?
     * @param Tag The tag to search for
     * @return Is the given tag present?
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = TerrainTags, meta = (AutoCreateRefTerm = Tag))
    bool HasTerrainTag(const FGameplayTag &Tag) const;

    /**
     * Does this actor have any of the given terrain tags?
     * @param Tags The tags to search for
     * @return Is at least a single terrain tag present?
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = TerrainTags, meta = (AutoCreateRefTerm = Tags))
    bool HasAnyTerrainTag(const FGameplayTagContainer &Tags) const;

    /**
     * Does this actor have all of the given terrain tags?
     * @param Tags The tags to search for
     * @return Are all the terrain tags present?
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = TerrainTags, meta = (AutoCreateRefTerm = Tags))
    bool HasAllTerrainTags(const FGameplayTagContainer &Tags) const;
};