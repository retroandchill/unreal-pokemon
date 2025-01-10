// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BlueprintMathUtils.generated.h"

/**
 * Common utility functions for math
 */
UCLASS()
class RETROLIB_API UBlueprintMathUtils : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Find the midpoint between two vector objects
     *
     * @param Start The first vector to check
     * @param End The second vector to check
     * @return The midpoint between the two vectors
     */
    UFUNCTION(BlueprintPure, Category = "Math|Vector")
    static FVector Midpoint(const FVector &Start, const FVector &End);

    /**
     * Returns a random integer within the specified range.
     *
     * @param Range An FInt32Range object that specifies the lower and upper bounds.
     * @return A random integer within the given range.
     */
    UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (AutoCreateRefTerm = Range))
    static int32 RandomIntInRange(const FInt32Range &Range);
};
