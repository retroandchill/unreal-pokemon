﻿// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Memory/GCPointer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Trainers/Trainer.h"
#include "PokemonSubsystem.generated.h"

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
	static UPokemonSubsystem &GetInstance();
	
	/**
	 * The the ID of the HP stat
	 * @return The stat used referring to a Pokémon's HP
	 */
	UFUNCTION(BlueprintPure, Category = "Display Names")
	FName GetHPStat() const;

	/**
	 * Get the player trainer
	 * @return A reference to the player trainer character
	 */
	ITrainer &GetPlayer();

	/**
	 * Get the player trainer
	 * @return A reference to the player trainer character
	 */
	const ITrainer &GetPlayer() const;

private:
	/**
	 * Singleton instance used to hold a backdoor reference to this system
	 */
	static UPokemonSubsystem *Instance;
	
	/**
	 * The stat used referring to a Pokémon's HP
	 */
	FName HPStat;

	/**
	 * The trainer that represents the player character
	 */
	TUniquePtr<ITrainer> Player;
	
};
