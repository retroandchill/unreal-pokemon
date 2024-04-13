// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SelectDataRow.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USelectDataRow : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for handling the selection of a row from a data object in the game.
 */
class POKEMONDATADEVELOPER_API ISelectDataRow {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Get the row names for the dropdown
     * @return The list of all valid row names
     */
    virtual TArray<FName> GetRowNames() const = 0;

    /**
     * Get the pin that represents the Row Name
     * @return The pin that lines up with the row name paramater
     */
    virtual UEdGraphPin *GetRowPin() const = 0;
};
