// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "TileSetTagRequirements.h"
#include "TileSet.generated.h"

USTRUCT()
struct GRIDMAP_API FGridMapTileBitset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint32 Bitset = 0;
};

USTRUCT()
struct GRIDMAP_API FGridMapTileList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGridMapTileBitset TileAdjacency;

	UPROPERTY(EditAnywhere)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, meta = (AllowAbstract))
	TArray<TSoftObjectPtr<class UStaticMesh>> Tiles;

	TSoftObjectPtr<class UStaticMesh> GetRandomTile() const;
};

/**
 * 
 */
UCLASS(BlueprintType)
class GRIDMAP_API UGridMapTileSet : public UObject
{
	GENERATED_BODY()
public:
	UGridMapTileSet();
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Tile Set Config")
	FGameplayTagContainer TileTags;

	UPROPERTY(EditDefaultsOnly, Category="Tile Set Config")
	FTileSetTagRequirements AdjacencyTagRequirements;

	UPROPERTY(EditDefaultsOnly, Category="Tile Set Config")
	bool bMatchesEmpty;

	UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
	uint32 TileSize;

	UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
	uint32 TileHeight;

	UPROPERTY(EditDefaultsOnly, Category="Tiles")
	TArray<FGridMapTileList> Tiles;

	const FGridMapTileList* FindTilesForAdjacency(uint32 bitmask) const;

protected:
	const FGridMapTileList* SearchForTilesWithCompatibleAdjacency(uint32 bitmask) const;

public:
	//this can't be here, it breaks non-editor builds :/
//#if WITH_EDITOR
	UPROPERTY(Transient)
	TObjectPtr<class UStaticMesh> MeshForIcon;
//#endif
};
