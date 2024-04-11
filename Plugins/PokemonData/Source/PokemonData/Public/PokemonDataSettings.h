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

  private:
    /**
     * The list of data tables used by the module
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetDataTables, Category = "Data Tables",
              meta = (AllowedClasses = "DataTable"))
    TSet<FSoftObjectPath> DataTables;
};
