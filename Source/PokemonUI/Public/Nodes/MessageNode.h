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
class POKEMONUI_API IMessageNode {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
