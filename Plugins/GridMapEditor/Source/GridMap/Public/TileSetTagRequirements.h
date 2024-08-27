#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TileSetTagRequirements.generated.h"

// nb:  Copied from GameplayEffectTypes to avoid placing a dependency on the GameplayAbilities plugin

USTRUCT(BlueprintType)
struct GRIDMAP_API FTileSetTagRequirements
{
	GENERATED_BODY()

	/** All of these tags must be present */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayModifier)
	FGameplayTagContainer RequireTags;

	/** None of these tags may be present */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayModifier)
	FGameplayTagContainer IgnoreTags;

	/** True if all required tags and no ignore tags found */
	bool	RequirementsMet(const FGameplayTagContainer& Container) const;

	/** True if neither RequireTags or IgnoreTags has any tags */
	bool	IsEmpty() const;

	/** Return debug string */
	FString ToString() const;
};
