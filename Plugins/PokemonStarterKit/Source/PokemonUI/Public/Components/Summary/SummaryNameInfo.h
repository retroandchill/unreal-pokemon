// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/PokemonInfoWidget.h"
#include "Pokemon/Breeding/PokemonGender.h"

#include "SummaryNameInfo.generated.h"

class UCommonTextStyle;
class UCommonNumericTextBlock;
class UCommonTextBlock;
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
    TMap<EPokemonGender, TSubclassOf<UCommonTextStyle>> GenderTextColors;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> PokemonNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonNumericTextBlock> PokemonLevelText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> PokemonGenderText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonBallIcon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonStatusIcon;
};
