// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "PokemonStorageScreen.generated.h"

class UStorageBoxWindow;
/**
 * The screen for managing depositing and withdrawing Pokémon from the boxes.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonStorageScreen : public UScreen {
    GENERATED_BODY()

public:
    /**
     * Get the box storage widget
     * @return The window that displays the contents of the current box
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UStorageBoxWindow* GetStorageBoxWindow() const {
        return StorageBoxWindow;
    }

private:
    /**
     * The window that displays the contents of the current box
     */
    UPROPERTY(BlueprintGetter = GetStorageBoxWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UStorageBoxWindow> StorageBoxWindow;

};
