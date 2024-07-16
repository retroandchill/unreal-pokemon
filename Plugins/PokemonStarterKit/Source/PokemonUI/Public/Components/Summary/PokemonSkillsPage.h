// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Summary/SummaryScreenPage.h"

#include "PokemonSkillsPage.generated.h"

class UHPStatRow;
class UDisplayText;
class UPokemonStatRow;
/**
 * The page that displays a Pokémon's stats and ability information.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonSkillsPage : public USummaryScreenPage {
    GENERATED_BODY()

  protected:
    TSharedRef<SWidget> RebuildWidget() override;
    void RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

    /**
     * Slot the selected HP stat row in the list of available stat rows.
     * @param Row The stat row to slot.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Slot HP Stat Row", Category = "Stats")
    void SlotHPStatRow(UHPStatRow *Row);

    /**
     * Slot the selected stat row in the list of available stat rows.
     * @param Row The stat row to slot.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats")
    void SlotStatRow(UPokemonStatRow *Row);

  private:
    /**
     * The actual class used to create the HP stat rows.
     */
    UPROPERTY(EditAnywhere, DisplayName = "HP Stat Row", Category = "Stats")
    TSubclassOf<UHPStatRow> HPStatRow;

    /**
     * The actual class used to create the stat rows.
     */
    UPROPERTY(EditAnywhere, Category = "Stats")
    TSubclassOf<UPokemonStatRow> StatRowClass;

    /**
     * Get the list of stats to display in the screen.
     */
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    TArray<FName> StatsToDisplay;

    /**
     * The list of stat rows that have been slotted.
     */
    UPROPERTY()
    TArray<TObjectPtr<UPokemonStatRow>> StatRows;

    /**
     * The widget that displays the name of the Pokémon's ability.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> AbilityName;

    /**
     * The widget that displays the description of the Pokémon's ability.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> AbilityDescription;
};
