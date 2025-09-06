// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Input/UIActionBindingHandle.h"

#include "PokemonMovesPage.generated.h"

class UInputAction;
/**
 * Delegate for the drawing of a Pokémon's moves.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDrawPokemonMoves, const TScriptInterface<IPokemon> &, Pokemon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckMoves);

/**
 * The page in the summary screen that displays the moves.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonMovesPage : public USummaryScreenPage
{
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;
    void OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) override;
    void OnPageShown() override;
    void OnPageHidden() override;

  private:
    UPROPERTY(BlueprintAssignable, Category = Events)
    FDrawPokemonMoves DrawPokemonMoves;

    UPROPERTY(BlueprintAssignable, Category = Events)
    FOnCheckMoves OnSelected;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> SelectionAction;

    FUIActionBindingHandle InspectActionHandler;

    UPROPERTY(EditAnywhere, Category = Input)
    FText ActionText;

    bool bIsShown = false;
};
