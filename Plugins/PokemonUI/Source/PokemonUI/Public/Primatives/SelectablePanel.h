// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Refreshable.h"
#include "UObject/Interface.h"
#include "SelectablePanel.generated.h"

class USelectableWidget;

// This class does not need to be modified.
UINTERFACE()
class USelectablePanel : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents a selectable panel in the world.
 */
class POKEMONUI_API ISelectablePanel : public IRefreshable {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Set the widget that owns this one
     * @param NewOwner The new owner widget
     */
    virtual void SetOwner(USelectableWidget *NewOwner) = 0;
};
