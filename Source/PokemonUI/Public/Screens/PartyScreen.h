// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PartyScreen.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPartyScreen : public UInterface {
	GENERATED_BODY()
};

/**
 * Generic screen class for any screens shown to the player
 */
class POKEMONUI_API IPartyScreen {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Begin the switching process.
	 * @param Index The index to start the switch at.
	 */
	virtual void BeginSwitch(int32 Index) = 0;

	/**
	 * Set the text of the help window
	 * @param Text The new text for the help window
	 */
	virtual void SetHelpText(const FText &Text) = 0;
};
