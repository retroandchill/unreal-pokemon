// Fill out your copyright notice in the Description page of Project Settings.

#include "GridMapStaticMeshActor.h"

AGridMapStaticMeshActor::AGridMapStaticMeshActor(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {
    GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
}

const FGridTileHandle & AGridMapStaticMeshActor::GetGridTile() const {
    return GridTile;
}

void AGridMapStaticMeshActor::SetGridTile(const FGridTileHandle &InGridTile) {
    GridTile = InGridTile;
}

int32 AGridMapStaticMeshActor::GetLayer() const {
    return CurrentLayer;
}

void AGridMapStaticMeshActor::SetLayer(int32 Layer) {
    CurrentLayer = Layer;
}