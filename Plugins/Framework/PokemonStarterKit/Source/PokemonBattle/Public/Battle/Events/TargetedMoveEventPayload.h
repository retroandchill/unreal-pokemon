// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "MoveEventPayload.h"
#include "UObject/Interface.h"

#include "TargetedMoveEventPayload.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTargetedMoveEventPayload : public UMoveEventPayload {
    GENERATED_BODY()
};

/**
 * Event payload that represents a move event for a single target
 */
class POKEMONBATTLE_API ITargetedMoveEventPayload : public IMoveEventPayload {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Get the target of the move
     * @return The move's target
     */
    virtual const TScriptInterface<IBattler> &GetTarget() const = 0;
};