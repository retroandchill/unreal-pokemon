// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TimeBlueprintLibrary.generated.h"

/**
 * @class UTimeBlueprintLibrary
 * @brief A utility class for handling time-related operations in Unreal Engine.
 *
 * This class is derived from UBlueprintFunctionLibrary and provides static functions that can be utilized
 * in Blueprints to work with time-related features.
 */
UCLASS()
class COMMONUTILITIES_API UTimeBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Retrieves the current date and time.
     *
     * This function provides the current system date and time as an FDateTime object.
     * It is designed to be called in Blueprints for time-related operations.
     *
     * @return The current date and time as an FDateTime object.
     */
    UFUNCTION(BlueprintPure, Category = "Time")
    static FDateTime GetDateTimeNow();
};
