// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Refreshable.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class INTERACTIVEUI_API URefreshable : public UInterface
{
    GENERATED_BODY()
};

/**
 * Represents a UI component that can be refreshed updating its contents on command.
 */
class INTERACTIVEUI_API IRefreshable
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Refresh the contents of the widget setting the values where appropriate
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual void Refresh() = 0;
};
