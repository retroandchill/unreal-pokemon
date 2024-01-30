// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EvolutionBase.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEvolutionBase : public UInterface {
	GENERATED_BODY()
};

/**
 * Marker interface used to denote that the class represents an evolution method, however the game wishes to use it.
 */
class POKEMONDATA_API IEvolutionBase {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
