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
	
};
