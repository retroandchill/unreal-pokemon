// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TimeBlueprintLibrary.generated.h"

/**
 *
 */
UCLASS()
class COMMONUTILITIES_API UTimeBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, Category = "Time")
    static FDateTime GetDateTimeNow();
};
