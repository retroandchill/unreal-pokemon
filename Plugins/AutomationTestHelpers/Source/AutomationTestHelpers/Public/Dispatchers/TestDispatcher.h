// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "TestDispatcher.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTestDispatcher : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface used for dispatching events to a blueprint class. Used mainly to test blueprint node execution.
 */
class AUTOMATIONTESTHELPERS_API ITestDispatcher {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Execute the test BP code.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Testing")
    void ExecuteTest();
};