// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DateUtils.generated.h"

/**
 * Utlity methods related to date operations.
 */
UCLASS()
class RETROLIB_API UDateUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Creates an FTimespan from a specified number of seconds.
     *
     * @param Seconds The number of seconds to convert into a FTimespan.
     * @return An FTimespan representing the given number of seconds.
     */
    UFUNCTION(BlueprintPure, Category = Timespan)
    static FTimespan FromSeconds(double Seconds);
};
