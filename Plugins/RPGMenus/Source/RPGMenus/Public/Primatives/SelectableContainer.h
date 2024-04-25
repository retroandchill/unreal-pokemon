// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableContainer.generated.h"

class USelectableOption;
// This class does not need to be modified.
UINTERFACE()
class USelectableContainer : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for container that contains selectable objects.
 */
class RPGMENUS_API ISelectableContainer {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Get the list of options used by the menu.
     * @return The list of options used by the menu
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Selection)
    TArray<USelectableOption*> GetOptions() const;

};