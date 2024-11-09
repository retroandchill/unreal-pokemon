// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/UserWidget.h"

#include "PokemonInfoWidget.generated.h"

class IPokemon;

/**
 * Abstract base class for a Widget that contains information about a particular Pokémon.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonInfoWidget : public UCommonUserWidget {
    GENERATED_BODY()

public:
    /**
     * Update the information about the Pokémon displayed by this widget.
     * 
     * @param NewPokemon The new Pokémon to display the info for
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Display)
    void Refresh(const TScriptInterface<IPokemon> &NewPokemon);

    /**
     * Retrieves the current Pokémon information displayed by the widget.
     * 
     * @return A reference to the current Pokémon.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Display)
    const TScriptInterface<IPokemon> &GetPokemon() const {
        return Pokemon;
    }

    /**
     * Sets a new Pokémon for the widget to display and updates the widget with the new Pokémon information.
     * 
     * @param NewPokemon The new Pokémon to be displayed in the widget.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Display)
    void SetPokemon(const TScriptInterface<IPokemon> &NewPokemon);

protected:
    /**
     * Called when a new Pokémon is set for this widget.
     * This event can be overridden in Blueprints to update the display or perform additional logic when the Pokémon is changed.
     * 
     * @param NewPokemon The new Pokémon that has been set for the widget.
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    void OnPokemonSet(const TScriptInterface<IPokemon> &NewPokemon);

private:
    /**
     * Interface reference to a Pokémon used by this widget.
     * Allows for displaying and updating information about a specific Pokémon.
     * This property is exposed to Blueprints with getter and setter functions.
     */
    UPROPERTY(BlueprintGetter = GetPokemon, BlueprintSetter = SetPokemon, Category = Display)
    TScriptInterface<IPokemon> Pokemon;
};