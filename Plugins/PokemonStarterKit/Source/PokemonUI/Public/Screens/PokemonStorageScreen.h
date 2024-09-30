// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "PokemonStorageScreen.generated.h"

class UStorageInfoPanel;
class IPokemon;
class UWidgetSwitcher;
class UStorageBoxWindow;

/**
 * The screen for managing depositing and withdrawing Pokémon from the boxes.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonStorageScreen : public UScreen {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;
    
public:
    /**
     * Get the box storage widget.
     * @return The window that displays the contents of the current box
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UStorageBoxWindow* GetStorageBoxWindow() const {
        return StorageBoxWindow;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UStorageInfoPanel* GetStorageInfoPanel() const {
        return StorageInfoPanel;
    }
    
private:
    void OnSelectedPokemonChanged(TOptional<IPokemon&> SelectedPokemon);
    
    /**
     * The window that displays the contents of the current box
     */
    UPROPERTY(BlueprintGetter = GetStorageBoxWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UStorageBoxWindow> StorageBoxWindow;

    UPROPERTY(BlueprintGetter = GetStorageInfoPanel, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UStorageInfoPanel> StorageInfoPanel;

};
