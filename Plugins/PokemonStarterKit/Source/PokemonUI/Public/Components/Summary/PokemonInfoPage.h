// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Trainers/TrainerGender.h"

#include "PokemonInfoPage.generated.h"

class UCommonTextStyle;
class UImage;
class UProgressBar;
class UDisplayText;

/**
 * The page used to display the Pokémon info.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonInfoPage : public USummaryScreenPage {
    GENERATED_BODY()

  protected:
    void OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) override;

    /**
     * Slot the supplied type icon into the widget.
     * @param Icon The icon to slot
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Visuals|Icons")
    void SlotTypeIcon(UImage *Icon);

    /**
     * Clear the existing type icons before drawing some new ones.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Visuals|Icons")
    void ClearTypeIcons();

  private:
    /**
     * The color settings for the gender text widget
     */
    UPROPERTY(EditAnywhere, Category = "Visuals|Text")
    TMap<ETrainerGender, TSubclassOf<UCommonTextStyle>> GenderTextColors;

    /**
     * The widget used to display the Pokédex number.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonNumberText;

    /**
     * The widget used to display the Pokémon's species.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> SpeciesNameText;

    /**
     * The widget used to display the Pokémon's OT name.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> OTNameText;

    /**
     * The widget used to display the Pokémon's ID.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonIDText;

    /**
     * The widget used to display the Pokémon's total Exp.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ExpTotalText;

    /**
     * The widget used to display the amount of Exp. needed for the Pokémon to level up.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> NextLevelUpCountText;

    /**
     * The widget used to visually display the amount of Exp. needed for the Pokémon to level up.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ExpBar;

    /**
     * The widget used to display the item's name
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ItemNameText;

    /**
     * The image used to display the item's icon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemIcon;

    /**
     * The widget used to display the item's description
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ItemDescriptionText;

    /**
     * The image used to if a Pokémon is shiny or not
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ShinyIcon;

    /**
     * The length of the Pokémon's Pokédex number
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin = 1))
    int32 PokedexNumberLength = 3;

    /**
     * The length of the Pokémon's ID number
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin = 1))
    int32 IdNumberLength = 6;

    /**
     * The text displayed if the Pokémon is holding no item
     */
    UPROPERTY(EditAnywhere, Category = Display)
    FText NoItemText = NSLOCTEXT("UPokemonInfoPage", "NoItemText", "None");
};
