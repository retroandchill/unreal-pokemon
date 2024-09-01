// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Terrain.h"
#include "Engine/StaticMeshActor.h"

#include "GridMapStaticMeshActor.generated.h"

/**
 *
 */
UCLASS()
class GRIDMAP_API AGridMapStaticMeshActor : public AStaticMeshActor, public ITerrain {
    GENERATED_BODY()

  public:
    explicit AGridMapStaticMeshActor(const FObjectInitializer &ObjectInitializer = FObjectInitializer());

    bool HasTerrainTag_Implementation(const FGameplayTag &Tag) const override;
    bool HasAnyTerrainTag_Implementation(const FGameplayTagContainer &TagsToCheck) const override;
    bool HasAllTerrainTags_Implementation(const FGameplayTagContainer &TagsToCheck) const override;

    UPROPERTY()
    TObjectPtr<class UGridMapTileSet> TileSet;

    UPROPERTY(VisibleAnywhere, Category = Layers)
    int32 CurrentLayer = 0;
};