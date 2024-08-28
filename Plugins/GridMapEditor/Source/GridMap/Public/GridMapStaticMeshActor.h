// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridMapTile.h"
#include "Engine/StaticMeshActor.h"

#include "GridMapStaticMeshActor.generated.h"

class UGridMapTileSet;


/**
 *
 */
UCLASS()
class GRIDMAP_API AGridMapStaticMeshActor : public AStaticMeshActor, public IGridMapTile {
    GENERATED_BODY()

  public:
    explicit AGridMapStaticMeshActor(const FObjectInitializer &ObjectInitializer = FObjectInitializer());

    const FGridTileHandle & GetGridTile() const final;
    void SetGridTile(const FGridTileHandle &InGridTile) final;
    int32 GetLayer() const final;
    void SetLayer(int32 Layer) final;

private:
    UPROPERTY()
    FGridTileHandle GridTile;
    
    UPROPERTY(VisibleAnywhere, Category = Layers)
    int32 CurrentLayer = 0;
};