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
	UPokemonCoreSettings();
	
	/**
	 * The the ID of the HP stat
	 * @return The stat used referring to a Pokémon's HP
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Display Names")
	FName GetHPStat() const;

	/**
	 * The maximum number of Pokémon a trainer can carry at any given time
	 * @return The maximum number of Pokémon a trainer can carry at any given time
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Party)
	int32 GetMaxPartySize() const;

	/**
	 * The class used for all Pokémon objects
	 * @return The class used for all Pokémon objects
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Classes")
	TSubclassOf<UObject> GetPokemonClass() const;

	/**
	 * The class used for all Stat Block objects
	 * @return The class used for all Stat Block objects
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Classes")
	TSubclassOf<UObject> GetStatBlockClass() const;

private:

	/**
	 * The stat used referring to a Pokémon's HP
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetHPStat, Config, DisplayName = "HP Stat", Category = "Display Names")
	FName HPStat;

	/**
	 * The maximum number of Pokémon a trainer can carry at any given time
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetMaxPartySize, Config, Category = Party, meta = (UIMin = 1, ClampMin = 1))
	int32 MaxPartySize;

	/**
	 * The class used for all Pokémon objects
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetPokemonClass, Config, AdvancedDisplay, Category = "Data Classes", meta = (MustImplement = Pokemon))
	TSubclassOf<UObject> PokemonClass;

	/**
	 * The class used for all Stat Block objects
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetStatBlockClass, Config, AdvancedDisplay, Category = "Data Classes",
		meta = (MustImplement = StatBlock))
	TSubclassOf<UObject> StatBlockClass;
};
