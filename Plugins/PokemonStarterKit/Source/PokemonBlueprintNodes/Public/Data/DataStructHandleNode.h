// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DataStructHandleNode.generated.h"

// This class does not need to be modified.
UINTERFACE()
class POKEMONBLUEPRINTNODES_API UDataStructHandleNode : public UInterface {
    GENERATED_BODY()
};

/**
 *
 */
class POKEMONBLUEPRINTNODES_API IDataStructHandleNode {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual UScriptStruct *GetStructType() const = 0;
};