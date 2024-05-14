// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SummaryScreenPage.generated.h"

class IPokemon;
/**
 * Abstract declaration of a page on thje summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryScreenPage : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Refresh the pages information with information from the given Pokémon.
     * @param Pokemon The Pokémon to refresh the information for
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Summary)
    void RefreshInfo(const TScriptInterface<IPokemon> &Pokemon);
};
