// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "GridMapEditorTypes.h"
#include "GridMapEditorUISettings.h"

class GRIDMAPEDITOR_API FGridMapEditorMode : public FEdMode
{
  protected:
    typedef TPair<class AGridMapStaticMeshActor *, uint32> FAdjacentTile;

  public:
    const static FEditorModeID EM_GridMapEditorModeId;

  public:
    FGridMapEditorMode();

    // FEdMode interface
    void Enter() override;
    void Exit() override;
    void Tick(FEditorViewportClient *ViewportClient, float DeltaTime) override;
    bool UsesToolkits() const override;
    void AddReferencedObjects(FReferenceCollector &Collector) override;

    bool StartTracking(FEditorViewportClient *InViewportClient, FViewport *InViewport) override;
    bool EndTracking(FEditorViewportClient *InViewportClient, FViewport *InViewport) override;
    bool MouseMove(FEditorViewportClient *ViewportClient, FViewport *Viewport, int32 MouseX, int32 MouseY) override;
    bool CapturedMouseMove(FEditorViewportClient *InViewportClient, FViewport *InViewport, int32 InMouseX,
                           int32 InMouseY) override;
    bool InputKey(FEditorViewportClient *InViewportClient, FViewport *InViewport, FKey InKey,
                  EInputEvent InEvent) override;
    bool HandleClick(FEditorViewportClient *InViewportClient, HHitProxy *HitProxy,
                     const FViewportClick &Click) override;

    bool Select(AActor *InActor, bool bInSelected) override;
    bool IsSelectionAllowed(AActor *InActor, bool bInSelection) const override;
    // End of FEdMode interface

    /** Return the current grid map editing state */
    EGridMapEditingState GetEditingState() const;

    /** Simgple wrapper to know if we can edit the grid map based on edit state */
    bool IsEditingEnabled() const
    {
        return GetEditingState() == EGridMapEditingState::Enabled;
    }

    void AddActiveTileSet(UGridMapTileSet *TileSet);
    const TArray<UGridMapTileSet *> &GetActiveTileSets() const;
    void SetActiveTileSet(UGridMapTileSet *TileSet);

    void UpdateAllTiles();

  private:
    void BindCommandList();
    void ClearAllToolSelection();
    void OnSetPaintTiles();
    void OnSetSelectTiles();
    void OnSetTileSettings();

    void GridMapBrushTrace(FEditorViewportClient *ViewportClient, const FVector &InRayOrigin,
                           const FVector &InRayDirection);

    int32 GetTileSize() const;
    int32 GetTileHeight() const;

    FVector SnapLocation(const FVector &InLocation);

    void PaintTile();
    void EraseTile(class AGridMapStaticMeshActor *TileToErase);

    uint32 GetTileAdjacencyBitmask(class UWorld *World, const FVector &Origin, UGridMapTileSet *TileSet,
                                   int32 Layer) const;
    bool TilesAt(class UWorld *World, const FVector &Origin, int32 Layer,
                 TArray<class AGridMapStaticMeshActor *> &OutTiles) const;
    bool GetAdjacentTiles(class UWorld *World, const FVector &Origin,
                          TArray<TPair<class AGridMapStaticMeshActor *, uint32>> &OutAdjacentTiles, int32 Layer,
                          bool bIncludeEmptyTiles = false) const;
    void UpdateAdjacentTiles(class UWorld *World, const TArray<FAdjacentTile> &RootActors);

    FString CreateActorLabel(const class UGridMapTileSet *TileSet) const;

  public:
    FGridMapEditorUISettings UISettings;

    /** Command list lives here so that the key bindings on the commands can be processed in the viewport. */
    TSharedPtr<FUICommandList> UICommandList;

  private:
    bool bIsPainting;

    bool bBrushTraceValid;
    FVector BrushLocation;
    FVector BrushTraceDirection;
    TWeakObjectPtr<class AActor> BrushTraceHitActor;
    TObjectPtr<UStaticMeshComponent> TileBrushComponent;
    /** The dynamic material of the tile brush. */
    UMaterialInstanceDynamic *BrushMID;
    FColor BrushDefaultHighlightColor;
    FColor BrushWarningHighlightColor;
    FColor BrushCurrentHighlightColor;

    TArray<UGridMapTileSet *> ActiveTileSets;
    UGridMapTileSet *ActiveTileSet;
};
