// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PokemonInfoWidget.generated.h"

class IPokemon;
/**
 * Abstract base class for a Widget that contains information about a particular Pokémon.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonInfoWidget : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Update the information about the Pokémon displayed by this widget.
     * @param Pokemon The new Pokémon to display the info for
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Display)
    void Refresh(const TScriptInterface<IPokemon> &Pokemon);
};
