// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Serializable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USerializable : public UInterface
{
    GENERATED_BODY()
};

/**
 * Marker interface that represents an object that can be serialized for save games.
 */
class ENHANCEDSAVEGAME_API ISerializable{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
    public :
};