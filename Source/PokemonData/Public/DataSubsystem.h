// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/DataTableProxy.h"
#include "DataRetrieval/GameData.h"
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
	 * Get the data table that contains data of the specified type
	 * @tparam T The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	template <typename T>
	const TDataTableProxy<T> &GetDataTable() const {
		auto StructClass = T::StaticStruct();
		check(DataTables.Contains(StructClass));

		auto TableOut = dynamic_cast<TDataTableProxy<T> *>(DataTables[StructClass].Get());
		check(TableOut != nullptr);

		return *TableOut;
	}

	/**
	 * Get the data table that contains data of the specified type
	 * @param StructType The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	const IGameData &GetDataTable(TObjectPtr<const UScriptStruct> StructType) const;

private:
	/**
	 * The list of data tables in the game
	 */
	TMap<TObjectPtr<const UScriptStruct>, TUniquePtr<IGameData>> DataTables;
	
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
