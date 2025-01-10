// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameplayTagUtilities.generated.h"

/**
 *
 */
UCLASS()
class RETROLIB_API UGameplayTagUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Request a direct child to the provided gameplay tag
     * @param Base The base tag to get the child from.
     * @param Suffix The suffix to add to the child
     * @return The composite gameplay tag (invalid if the requested tag does not exist)
     */
    UFUNCTION(BlueprintPure, Category = "Utilities|GameplayTags", meta = (AutoCreateRefTerm = Base))
    static FGameplayTag RequestGameplayTagChild(const FGameplayTag &Base, FName Suffix);
};
