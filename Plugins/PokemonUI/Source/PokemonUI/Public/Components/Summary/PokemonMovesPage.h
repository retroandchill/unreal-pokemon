// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SummaryScreenPage.h"

#include "PokemonMovesPage.generated.h"

/**
 * Delegate for the drawing of a Pokémon's moves.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDrawPokemonMoves, const TScriptInterface<IPokemon> &, Pokemon);

/**
 * The page in the summary screen that displays the moves.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonMovesPage : public USummaryScreenPage {
    GENERATED_BODY()

  protected:
    void RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;
    bool CanSelectOnPage_Implementation() const override;

  private:
    UPROPERTY(BlueprintAssignable, Category = Events)
    FDrawPokemonMoves DrawPokemonMoves;
};
