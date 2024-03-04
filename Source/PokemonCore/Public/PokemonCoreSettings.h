// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PokemonCoreSettings.generated.h"

/**
 * Configuration class for the core Pokémon data
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Pokémon Core Settings"))
class POKEMONCORE_API UPokemonCoreSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	/**
	 * The the ID of the HP stat
	 * @return The stat used referring to a Pokémon's HP
	 */
	UFUNCTION(BlueprintPure, Category = "Display Names", meta = (BlueprintInternalUseOnly))
	FName GetHPStat() const;

private:
	/**
	 * The stat used referring to a Pokémon's HP
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetHPStat, Config, DisplayName = "HP Stat", Category = "Display Names")
	FName HPStat;
};
