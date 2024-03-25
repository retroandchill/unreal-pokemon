﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GridBased2DSettings.generated.h"

/**
 * The Settings class for the Grid-Based 2D Module
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Grid-Based 2D Settings"))
class GRIDBASED2D_API UGridBased2DSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	/**
	 * Get the size of the grid in the game
	 * @return The size of the grid according to the game
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Scale")
	double GetGridSize() const;

	/**
	 * Get the dynamic offset of a level in the game
	 * @return The dynamic offset of a level in the game
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Level Streaming")
	FVector GetDynamicLevelOffset() const;

	/**
	 * Get the dynamic offset of a level in the game
	 * @return The dynamic offset of a level in the game
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Tilesets")
	const FSoftObjectPath& GetTerrainTagDataTable() const;

private:
	/**
	 * The size of the grid according to the game
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetGridSize, Config, Category = "Scale")
	double GridSize = 32;
	
	/**
	 * The dynamic offset of a level in the game
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetDynamicLevelOffset, Config, Category = "Level Streaming")
	FVector DynamicLevelOffset;

	/**
	 * The data table asset used for terrain tag mappings, for various movement types and other traits
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetTerrainTagDataTable, Config, Category = "Tilesets")
	FSoftObjectPath TerrainTagDataTable;
};
