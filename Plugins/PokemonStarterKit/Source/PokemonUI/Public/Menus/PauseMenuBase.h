// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonMenuBase.h"
#include "PauseMenuBase.generated.h"

class IPokemon;
class UPokemonInfoButtonBase;
/**
 * The base class for the pause menu in the game.
 */
UCLASS(Abstract)
class POKEMONUI_API UPauseMenuBase : public UPokemonMenuBase {
    GENERATED_BODY()

public:
    void NativeConstruct() override;

protected:
    /**
     * Slot the button for a given Pokémon into the widget
     * @param Button The button to place into the slot
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Pokémon")
    void SlotPokemonButton(UPokemonInfoButtonBase* Button);

private:
    UPokemonInfoButtonBase* CreatePokemonPanel(const TScriptInterface<IPokemon>& Pokemon);
    
    /**
     * The class used for a Pokémon panel
     */
    UPROPERTY(EditAnywhere, Category = "Pokémon")
    TSubclassOf<UPokemonInfoButtonBase> PokemonPanelClass;

    /**
     * The category used to display the information to the player
     */
    UPROPERTY()
    TArray<TObjectPtr<UPokemonInfoButtonBase>> PokemonPanels;

};
