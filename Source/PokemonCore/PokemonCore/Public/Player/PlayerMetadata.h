// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerMetadata.generated.h"

/**
 * Various playthrough related metadata for the player character, such as playtime.
 */
UCLASS()
class POKEMONCORE_API UPlayerMetadata : public UObject, public FTickableGameObject {
	GENERATED_BODY()

public:
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override;
	UWorld* GetTickableGameObjectWorld() const override;
	
	/**
	 * Get the datetime the game was started on.
	 * @return The datetime the player started the game on.
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Metadata")
	const FDateTime& GetStartDate() const;
	
	/**
	 * Retrieve the player's total playtime
	 * @return The total amount of time (in seconds) that the game has been played
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Metadata")
	float GetTotalPlaytime() const;

private:
	/**
	 * Start a brand new game
	 */
	void StartNewGame();
	
	/**
	 * The datetime the player started the game on.
	 */
	UPROPERTY(BlueprintGetter = GetStartDate, Category = "Player|Metadata")
	FDateTime StartDate;
	
	/**
	 * The total amount of time (in seconds) that the game has been played
	 */
	UPROPERTY(BlueprintGetter = GetTotalPlaytime, Category = "Player|Metadata")
	float TotalPlaytime;

	friend class UPokemonSubsystem;
};
