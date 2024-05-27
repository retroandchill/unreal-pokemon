// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonSettings.generated.h"

/**
 * General settings related to Pokémon.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Pokémon Settings"))
class POKEMONCORE_API UPokemonSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The the ID of the HP stat
     * @return The stat used referring to a Pokémon's HP
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Stats)
    FName GetHPStat() const;

    /**
     * Get the odds of a Pokémon being shiny.
     * @return The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    uint32 GetShinyChance() const;

    /**
     * The class default Poké Ball used a Pokémon
     * @return The item used for a Pokémon with no ball specified
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Miscellaneous)
    FName GetDefaultPokeBall() const;

  private:
    /**
     * The stat used referring to a Pokémon's HP
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetHPStat, Config, DisplayName = "HP Stat", Category = Stats,
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    FName HPStat = "HP";

    /**
     * The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    UPROPERTY(EditAnywhere, Config, Category = Miscellaneous)
    uint32 ShinyChance = 16;

    /**
     * The maximum number of moves a Pokémon can know.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDefaultPokeBall, Config, Category = Miscellaneous,
              meta = (GetOptions = "PokemonData.ItemHelper.GetPokeBallNames"))
    FName DefaultPokeBall = "POKEBALL";
};