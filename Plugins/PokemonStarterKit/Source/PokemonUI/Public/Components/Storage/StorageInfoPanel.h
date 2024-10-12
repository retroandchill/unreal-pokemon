// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "StorageInfoPanel.generated.h"

class IPokemon;
class UWidgetSwitcher;
/**
 * The side panel for the current Pokémon that is being selected in the box view.
 */
UCLASS(Abstract)
class POKEMONUI_API UStorageInfoPanel : public UCommonUserWidget {
    GENERATED_BODY()

public:
    /**
     * Apply any changes to the selected Pokémon to the Blueprints
     * @param Pokemon The new Pokémon to display the info for
     */
    UFUNCTION(BlueprintNativeEvent, Category = Storage)
    void SetSelectedPokemon(const TScriptInterface<IPokemon>& Pokemon);

    /**
     * Get the storage info switcher.
     * @return The widget switcher that displays the information about the Pokémon being displayed.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UWidgetSwitcher* GetStorageInfoSwitcher() const {
        return StorageInfoSwitcher;
    }

private:
    UPROPERTY()
    TScriptInterface<IPokemon> CurrentPokemon;
    
    /**
     * The widget switcher that displays the information about the Pokémon being displayed.
     */
    UPROPERTY(BlueprintGetter = GetStorageInfoSwitcher, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> StorageInfoSwitcher;
    
};
