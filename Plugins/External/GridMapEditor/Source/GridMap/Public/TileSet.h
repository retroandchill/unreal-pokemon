// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Ranges/Optional/OptionalRef.h"
#include "TileSetTagRequirements.h"
#include "UObject/Object.h"

#include "TileSet.generated.h"

USTRUCT()
struct GRIDMAP_API FGridMapTileBitset {
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere)
    uint32 Bitset = 0;
};

USTRUCT()
struct GRIDMAP_API FGridMapTileList {
    GENERATED_BODY()

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
class GRIDMAP_API UGridMapTileSet : public UObject {
    GENERATED_BODY()
  public:
    UGridMapTileSet();

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
    FGameplayTagContainer TileTags;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
    FTileSetTagRequirements AdjacencyTagRequirements;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
    bool bMatchesEmpty;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
    uint32 TileSize;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
    uint32 TileHeight;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config", meta = (UIMin = 1, ClampMin = 1))
    uint32 SizeX = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config", meta = (UIMin = 1, ClampMin = 1))
    uint32 SizeY = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config", meta = (UIMin = 1, ClampMin = 1))
    uint32 SizeZ = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Set Config")
    FVector2D BrushOffset;

    UPROPERTY(EditDefaultsOnly, Category = "Tiles")
    TArray<FGridMapTileList> Tiles;

    TOptional<const FGridMapTileList &> FindTilesForAdjacency(uint32 Bitmask) const;

  protected:
    TOptional<const FGridMapTileList &> SearchForTilesWithCompatibleAdjacency(uint32 Bitmask) const;

  public:
    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> MeshForIcon;
};
