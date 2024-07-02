// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MoveEventPayload.generated.h"

class IBattler;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class UMoveEventPayload : public UInterface {
    GENERATED_BODY()
};

/**
 * Event payload that represents a move event
 */
class POKEMONBATTLE_API IMoveEventPayload {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Get the user of a move
     * @return The user of the move
     */
    virtual const TScriptInterface<IBattler>& GetUser() const = 0;

};