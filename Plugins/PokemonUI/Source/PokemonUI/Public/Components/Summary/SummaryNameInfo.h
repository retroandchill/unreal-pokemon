// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/PokemonInfoWidget.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "Text/TextColor.h"

#include "SummaryNameInfo.generated.h"

class UImage;
class UDisplayText;
/**
 * Widget that describes a Pokémon's name
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryNameInfo : public UPokemonInfoWidget {
    GENERATED_BODY()

protected:
    void Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

private:
    /**
     * The color settings for the gender text widget
     */
    UPROPERTY(EditAnywhere, Category = "Visuals|Text")
    TMap<EPokemonGender, FTextColor> GenderTextColors;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonLevelText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonGenderText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonBallIcon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonStatusIcon;

};
