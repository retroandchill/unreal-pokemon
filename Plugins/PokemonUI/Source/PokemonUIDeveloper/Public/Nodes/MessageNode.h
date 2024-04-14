// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "MessageNode.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMessageNode : public UInterface {
    GENERATED_BODY()
};

/**
 * Marker interface used to denote a node that contains a message.
 */
class POKEMONUIDEVELOPER_API IMessageNode {
    GENERATED_BODY()
};
