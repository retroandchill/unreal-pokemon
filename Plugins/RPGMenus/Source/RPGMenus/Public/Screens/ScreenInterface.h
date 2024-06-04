// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UScreenInterface : public UInterface {
    GENERATED_BODY()
};

/**
 * Abstract interface definition of a screen object
 */
class RPGMENUS_API IScreenInterface {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Close the screen and return to the previous one
     */
    UFUNCTION(BlueprintCallable, Category = Navigation)
    virtual void CloseScreen() = 0;

};