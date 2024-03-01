// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Refreshable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URefreshable : public UInterface {
	GENERATED_BODY()
};

/**
 * Represents a UI component that can be refreshed updating its contents on command.
 */
class RPGMENUS_API IRefreshable {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**
	 * Refresh the contents of the widget setting the values where appropriate
	 */
	virtual void Refresh() = 0;
};
