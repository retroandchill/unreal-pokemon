// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "RunningMessageSetPayload.generated.h"

struct FRunningMessageSet;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class URunningMessageSetPayload : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents an event payload that contains a running message set
 */
class POKEMONBATTLE_API IRunningMessageSetPayload {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Get the running message set passed to the event in question
     * @return The running message set passed to the event
     */
    virtual const FRunningMessageSet &GetRunningMessageSet() const = 0;
};