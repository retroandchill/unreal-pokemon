// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PartyDisplayScreen.generated.h"

class UPokemonSelectionPaneBase;

// This class does not need to be modified.
UINTERFACE()
class UPartyDisplayScreen : public UInterface
{
    GENERATED_BODY()
};

/**
 * Represents a screen that displays a party selection pane of some kind.
 */
class POKEMONUI_API IPartyDisplayScreen
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    UFUNCTION(BlueprintNativeEvent, Category = Widgets)
    UPokemonSelectionPaneBase *GetPokemonSelectionPane() const;
};