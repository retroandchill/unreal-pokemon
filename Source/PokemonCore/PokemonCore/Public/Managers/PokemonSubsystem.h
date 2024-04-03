// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Trainers/Trainer.h"
#include "PokemonSubsystem.generated.h"

namespace Exp {
	class IGrowthRate;
}

/**
 * Subsystem for interfacing with the Pokémon data. Has a backdoor static pointer for non-UObject singleton access.
 */
UCLASS()
class POKEMONCORE_API UPokemonSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	/**
	 * Get the singleton instance of the Pokémon Manager class
	 * @return The singleton instance of the Pokémon Manager class
	 */
	static UPokemonSubsystem& GetInstance();

	/**
	 * Does the singleton instance exist?
	 * @return Does the singleton instance exist?
	 */
	static bool Exists();

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
	 * Get the player trainer
	 * @return A reference to the player trainer character
	 */
	ITrainer& GetPlayer();

	/**
	 * Get the player trainer
	 * @return A reference to the player trainer character
	 */
	const ITrainer& GetPlayer() const;

	/**
	 * Get the growth rate for the given name
	 * @param GrowthRate The growth rate to look up
	 * @return The found GrowthRate
	 */
	const Exp::IGrowthRate& GetGrowthRate(FName GrowthRate) const;

private:
	/**
	 * Singleton instance used to hold a backdoor reference to this system
	 */
	static UPokemonSubsystem* Instance;

	/**
	 * The stat used referring to a Pokémon's HP
	 */
	UPROPERTY(BlueprintGetter=GetHPStat, DisplayName = "HP Stat", Category = "Display Names")
	FName HPStat;

	/**
	 * The maximum number of Pokémon a trainer can carry at any given time
	 */
	UPROPERTY(BlueprintGetter=GetMaxPartySize, Category = Party)
	int32 MaxPartySize;

	/**
	 * The trainer that represents the player character
	 */
	UPROPERTY()
	TScriptInterface<ITrainer> Player;

	/**
	 * The list of GrowthRate objects used by the game
	 */
	TMap<FName, TUniquePtr<Exp::IGrowthRate>> GrowthRates;
};
