// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonStatRow.h"

#include "HPStatRow.generated.h"

/**
 * The specific stat row for a Pokémon's HP.
 */
UCLASS(Abstract, DisplayName = "HP Stat Row")
class POKEMONUI_API UHPStatRow : public UPokemonStatRow {
    GENERATED_BODY()

  protected:
    void SetStatValueText_Implementation(const TScriptInterface<IPokemon> &Pokemon,
                                         const TScriptInterface<IStatEntry> &StatInfo) override;

  private:
    /**
     * The widget that displays the Pokémon's HP as a rate.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;
};
