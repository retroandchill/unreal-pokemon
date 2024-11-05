// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestServiceBlueprintNodeExecutor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTestServiceBlueprintNodeExecutor : public UInterface {
    GENERATED_BODY()
};

/**
 * 
 */
class UNREALINJECTORTESTS_API ITestServiceBlueprintNodeExecutor {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintImplementableEvent, Category=UnitTesting, meta = (WorldContext = "WorldContext"))
    void SetTestValue(const UObject* WorldContext, int32 Value);

};