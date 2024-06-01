// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

/**
 * Generic screen class for any screens shown to the player
 */
class POKEMONUI_API IPartyScreen {
  public:
    virtual ~IPartyScreen() = default;

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

    virtual APlayerController &GetPlayerController() = 0;
};
