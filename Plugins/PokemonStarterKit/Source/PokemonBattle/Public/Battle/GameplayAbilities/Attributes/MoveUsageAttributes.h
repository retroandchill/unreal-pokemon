// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "MoveUsageAttributes.generated.h"

/**
 * Attributes that come into existence during move execution and disappear after the move has been resolved.
 */
UCLASS()
class POKEMONBATTLE_API UMoveUsageAttributes : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UMoveUsageAttributes, BasePower)
    ATTRIBUTE_ACCESSORS(UMoveUsageAttributes, Accuracy)
    ATTRIBUTE_ACCESSORS(UMoveUsageAttributes, Category)
    
private:
    UPROPERTY()
    FGameplayAttributeData BasePower;
    
    UPROPERTY()
    FGameplayAttributeData Accuracy;

    UPROPERTY()
    FGameplayAttributeData Category;

};
