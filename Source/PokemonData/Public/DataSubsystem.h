// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

struct FGrowthRateData;
struct FGenderRatio;
struct FEggGroup;
struct FBodyShape;
struct FBodyColor;
struct FHabitat;
struct FEvolutionData;
struct FStat;
struct FNature;
struct FStatus;
struct FWeather;
struct FEncounterType;
struct FEnvironment;
struct FBattleWeather;
struct FBattleTerrain;
struct FTarget;

struct FItem;

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

	/**
	 * Get a Gender Ratio data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Gender Ratio")
	const FGenderRatio &GetGenderRatio(FName ID) const;

	/**
	 * Get the list of IDs in the Gender Ratio table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Gender Ratio IDs",  Category = "Data/Hardcoded/Gender Ratio")
	TArray<FName> GetGenderRatioIds() const;

	/**
	 * Check if the provided Gender Ratio ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Gender Ratio defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Gender Ratio ID Valid?", Category = "Data/Hardcoded/Gender Ratio")
	bool IsGenderRatioIdValid(FName ID) const;

	/**
	 * Get an Egg Group data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Egg Group")
	const FEggGroup &GetEggGroup(FName ID) const;

	/**
	 * Get the list of IDs in the Egg Group table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Egg Group IDs",  Category = "Data/Hardcoded/Egg Group")
	TArray<FName> GetEggGroupIds() const;

	/**
	 * Check if the provided Egg Group ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Egg Group defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Egg Group ID Valid?", Category = "Data/Hardcoded/Egg Group")
	bool IsEggGroupIdValid(FName ID) const;

	/**
	 * Get an Body Shape data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Body Shape")
	const FBodyShape &GetBodyShape(FName ID) const;

	/**
	 * Get the list of IDs in the Body Shape table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Body Shape IDs",  Category = "Data/Hardcoded/Body Shape")
	TArray<FName> GetBodyShapeIds() const;

	/**
	 * Check if the provided Body Shape ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Body Shape defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Body Shape ID Valid?", Category = "Data/Hardcoded/Body Shape")
	bool IsBodyShapeIdValid(FName ID) const;

	/**
	 * Get an Body Color data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Body Color")
	const FBodyColor &GetBodyColor(FName ID) const;

	/**
	 * Get the list of IDs in the Body Color table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Body Color IDs",  Category = "Data/Hardcoded/Body Color")
	TArray<FName> GetBodyColorIds() const;

	/**
	 * Check if the provided Body Color ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Body Color defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Body Color ID Valid?", Category = "Data/Hardcoded/Body Color")
	bool IsBodyColorIdValid(FName ID) const;

	/**
	 * Get an Habitat data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Habitat")
	const FHabitat &GetHabitat(FName ID) const;

	/**
	 * Get the list of IDs in the Habitat table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Habitat IDs",  Category = "Data/Hardcoded/Habitat")
	TArray<FName> GetHabitatIds() const;

	/**
	 * Check if the provided Habitat ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Habitat defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Habitat ID Valid?", Category = "Data/Hardcoded/Habitat")
	bool IsHabitatIdValid(FName ID) const;


	/**
	 * Get an Evolution data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Evolution")
	const FEvolutionData &GetEvolution(FName ID) const;

	/**
	 * Get the list of IDs in the Evolution table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Evolution IDs",  Category = "Data/Hardcoded/Evolution")
	TArray<FName> GetEvolutionIds() const;

	/**
	 * Check if the provided Evolution ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Evolution defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Evolution ID Valid?", Category = "Data/Hardcoded/Evolution")
	bool IsEvolutionIdValid(FName ID) const;

	/**
	 * Get an Stat data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Stat")
	const FStat &GetStat(FName ID) const;

	/**
	 * Get the list of IDs in the Stat table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Stat IDs",  Category = "Data/Hardcoded/Stat")
	TArray<FName> GetStatIds() const;

	/**
	 * Check if the provided Stat ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Stat defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Stat ID Valid?", Category = "Data/Hardcoded/Stat")
	bool IsStatIdValid(FName ID) const;

	/**
	 * Get an Nature data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Nature")
	const FNature &GetNature(FName ID) const;

	/**
	 * Get the list of IDs in the Nature table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Nature IDs",  Category = "Data/Hardcoded/Nature")
	TArray<FName> GetNatureIds() const;

	/**
	 * Check if the provided Nature ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Nature defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Nature ID Valid?", Category = "Data/Hardcoded/Nature")
	bool IsNatureIdValid(FName ID) const;

	/**
	 * Get an Status data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Status")
	const FStatus &GetStatus(FName ID) const;

	/**
	 * Get the list of IDs in the Status table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Status IDs",  Category = "Data/Hardcoded/Status")
	TArray<FName> GetStatusIds() const;

	/**
	 * Check if the provided Status ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Status defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Status ID Valid?", Category = "Data/Hardcoded/Status")
	bool IsStatusIdValid(FName ID) const;

	/**
	 * Get an Weather data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Weather")
	const FWeather &GetWeather(FName ID) const;

	/**
	 * Get the list of IDs in the Weather table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Weather IDs",  Category = "Data/Hardcoded/Weather")
	TArray<FName> GetWeatherIds() const;

	/**
	 * Check if the provided Weather ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Weather defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Weather ID Valid?", Category = "Data/Hardcoded/Weather")
	bool IsWeatherIdValid(FName ID) const;

	/**
	 * Get an Encounter Type data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Encounter Type")
	const FEncounterType &GetEncounterType(FName ID) const;

	/**
	 * Get the list of IDs in the Encounter Type table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Encounter Type IDs",  Category = "Data/Hardcoded/Encounter Type")
	TArray<FName> GetEncounterTypeIds() const;

	/**
	 * Check if the provided Encounter Type ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Encounter Type defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Encounter Type ID Valid?", Category = "Data/Hardcoded/Encounter Type")
	bool IsEncounterTypeIdValid(FName ID) const;

	/**
	 * Get an Environment data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Environment")
	const FEnvironment &GetEnvironment(FName ID) const;

	/**
	 * Get the list of IDs in the Environment table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Environment IDs",  Category = "Data/Hardcoded/Environment")
	TArray<FName> GetEnvironmentIds() const;

	/**
	 * Check if the provided Environment ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Environment defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Environment ID Valid?", Category = "Data/Hardcoded/Environment")
	bool IsEnvironmentIdValid(FName ID) const;

	/**
	 * Get an Battle Weather data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Battle Weather")
	const FBattleWeather &GetBattleWeather(FName ID) const;

	/**
	 * Get the list of IDs in the Battle Weather table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Battle Weather IDs",  Category = "Data/Hardcoded/Battle Weather")
	TArray<FName> GetBattleWeatherIds() const;

	/**
	 * Check if the provided Battle Weather ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Battle Weather defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Battle Weather ID Valid?", Category = "Data/Hardcoded/Battle Weather")
	bool IsBattleWeatherIdValid(FName ID) const;

	/**
	 * Get an Battle Terrain data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Battle Terrain")
	const FBattleTerrain &GetBattleTerrain(FName ID) const;

	/**
	 * Get the list of IDs in the Battle Terrain table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Battle Terrain IDs",  Category = "Data/Hardcoded/Battle Terrain")
	TArray<FName> GetBattleTerrainIds() const;

	/**
	 * Check if the provided Battle Terrain ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Battle Terrain defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Battle Terrain ID Valid?", Category = "Data/Hardcoded/Battle Terrain")
	bool IsBattleTerrainIdValid(FName ID) const;

	/**
	 * Get an Target data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/Hardcoded/Target")
	const FTarget &GetTarget(FName ID) const;

	/**
	 * Get the list of IDs in the Target table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Target IDs",  Category = "Data/Hardcoded/Target")
	TArray<FName> GetTargetIds() const;

	/**
	 * Check if the provided Target ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Target defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Target ID Valid?", Category = "Data/Hardcoded/Target")
	bool IsTargetIdValid(FName ID) const;

	/**
	 * Get an Item data row by ID
	 * @param ID The ID to get the data of (Must be valid or else the game will crash)
	 * @return A reference to the retrieved data
	 */
	UFUNCTION(BlueprintPure, Category = "Data/PBS/Item")
	const FItem &GetItem(FName ID) const;

	/**
	 * Get the list of IDs in the Item table
	 * @return The list of names in the database
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Item IDs",  Category = "Data/PBS/Item")
	TArray<FName> GetItemIds() const;

	/**
	 * Check if the provided Item ID is valid or not
	 * @param ID The ID to check against
	 * @return If there is a Item defined with the provided ID
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Is Item ID Valid?", Category = "Data/PBS/Item")
	bool IsItemIdValid(FName ID) const;
	

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
