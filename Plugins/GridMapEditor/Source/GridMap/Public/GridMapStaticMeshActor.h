// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GridMapStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class GRIDMAP_API AGridMapStaticMeshActor : public AStaticMeshActor {
    GENERATED_BODY()

public:
    explicit AGridMapStaticMeshActor(const FObjectInitializer &ObjectInitializer = FObjectInitializer());

    UPROPERTY()
    TObjectPtr<class UGridMapTileSet> TileSet;

    UPROPERTY(VisibleAnywhere, Category = Layers)
    int32 CurrentLayer = 0;
};