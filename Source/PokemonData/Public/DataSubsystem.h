// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

struct FGrowthRateData;
/**
 * Persistent subsystem used to get the data tables
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UDataSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UDataSubsystem();


	/**
	 * Get a Growth Rate data row by ID
	 * @param ID The ID to get the growth rate from. (Must be valid or else the game will crash)
	 * @return A reference to the retrieved Growth Rate row
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Growth Rate")
	const FGrowthRateData &GetGrowthRate(FName ID) const;

	/**
	 * Get the list of IDs in the Growth Rate table
	 * @return The list of names in the growth rate table
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Growth Rate IDs",  Category = "Data/Hardcoded/Growth Rate")
	TArray<FName> GetGrowthRateIds() const;

	/**
	 * Check if the provided Growth Rate ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Growth Rate defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Growth Rate ID Valid?", Category = "Data/Hardcoded/Growth Rate")
	bool IsGrowthRateIdValid(FName ID) const;
	

private:
	/**
	 * The data table used for loading the Growth Rates
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *GrowthRateTable;

	/**
	 * The data table used for loading the Gender Ratios
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *GenderRatioTable;

	/**
	 * The data table used for loading the Egg Groups
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *EggGroupTable;

	/**
	 * The data table used for loading the Body Shapes
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *BodyShapeTable;

	/**
	 * The data table used for loading the Body Colors
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *BodyColorTable;

	/**
	 * The data table used for loading the Habitats
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *HabitatTable;

	/**
	 * The data table used for loading the Evolution Methods
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *EvolutionTable;

	/**
	 * The data table used for loading the Stats
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *StatTable;
	
	/**
	 * The data table used for loading the Natures
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *NatureTable;

	/**
	 * The data table used for loading the Statuses
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *StatusTable;

	/**
	 * The data table used for loading the field Weather effects
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *WeatherTable;

	/**
	 * The data table used for loading the Encounter Types
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *EncounterTypeTable;

	/**
	 * The data table used for loading the battle Environments
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *EnvironmentTable;

	/**
	 * The data table used for loading the in-battle Weather effects
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *BattleWeatherTable;

	/**
	 * The data table used for loading the in-battle Terrain effects
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *BattleTerrainTable;

	/**
	 * The data table used for loading the move Target options
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Hardcoded")
	UDataTable *TargetTable;

	/**
	 * The data table used for loading the Items obtainable in the game
	 */
	UPROPERTY(EditDefaultsOnly, Category = "PBS")
	UDataTable *ItemTable;
};
