// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonDataSettings.generated.h"

/**
 * Settings for the Pokémon Data module
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Pokémon Data Settings"))
class POKEMONDATA_API UPokemonDataSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The list of data tables used by the module
     * @return The list of data tables used by the module
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Tables")
    const TSet<FSoftObjectPath> &GetDataTables() const;

    /**
     * The maximum number of abilities a Pokémon can have.
     * @return The maximum number of abilities a Pokémon can have.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Party)
    int32 GetMaxDefaultAbilities() const;

  private:
    /**
     * The list of data tables used by the module
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetDataTables, Category = "Data Tables",
              meta = (AllowedClasses = "DataTable"))
    TSet<FSoftObjectPath> DataTables;

    /**
     * The maximum number of abilities a Pokémon can have.
     * <p>If a Pokémon has less abilities here, the last ability in the list is duplicated.</p>
     * <p>If the list has more than the required amount, then the list is truncated, and a warning is emitted.</p>
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxDefaultAbilities, Config, Category = Abilities,
              meta = (UIMin = 1, ClampMin = 1))
    int32 MaxDefaultAbilities = 2;
};
