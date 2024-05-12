// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "PokemonSummaryScreen.generated.h"

class IPokemon;
class USummaryPages;
class USummaryNameInfo;
class UImage;
/**
 * Screen for displaying a Pokémon's summary.
 */
UCLASS()
class POKEMONUI_API UPokemonSummaryScreen : public UScreen {
    GENERATED_BODY()

public:
    void SetPokemon(const TScriptInterface<IPokemon>& Pokemon);

private:
    /**
     * The widget containing the name information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USummaryNameInfo> NameInfo;

    /**
     * The widget that displays the Pokémon sprite
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonSprite;

    /**
     * The widget containing the name information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USummaryPages> SummaryPages;
};
