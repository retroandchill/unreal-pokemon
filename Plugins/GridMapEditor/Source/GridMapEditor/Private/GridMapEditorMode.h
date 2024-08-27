// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "GridMapEditorTypes.h"
#include "GridMapEditorUISettings.h"

class FGridMapEditorMode : public FEdMode
{
protected:
	typedef TPair<class AGridMapStaticMeshActor*, uint32> FAdjacentTile;

public:
	const static FEditorModeID EM_GridMapEditorModeId;

public:
	FGridMapEditorMode();
	virtual ~FGridMapEditorMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 MouseX, int32 MouseY) override;
	virtual bool CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;
	virtual bool InputKey(FEditorViewportClient* InViewportClient, FViewport* InViewport, FKey InKey, EInputEvent InEvent) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click) override;	

	virtual bool Select(AActor* InActor, bool bInSelected) override;
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;
	// End of FEdMode interface

	/** Return the current grid map editing state */
	EGridMapEditingState GetEditingState() const;

	/** Simgple wrapper to know if we can edit the grid map based on edit state */
	bool IsEditingEnabled() const
	{
		return GetEditingState() == EGridMapEditingState::Enabled;
	}

	void AddActiveTileSet(class UGridMapTileSet* TileSet);
	const TArray<class UGridMapTileSet*>& GetActiveTileSets() const;
	void SetActiveTileSet(class UGridMapTileSet* TileSet);

	void UpdateAllTiles();

private:
	void BindCommandList();
	void ClearAllToolSelection();
	void OnSetPaintTiles();
	void OnSetSelectTiles();
	void OnSetTileSettings();

	void GridMapBrushTrace(FEditorViewportClient* ViewportClient, const FVector& InRayOrigin, const FVector& InRayDirection);
	
	int32 GetTileSize() const;
	int32 GetTileHeight() const;

	FVector SnapLocation(const FVector& InLocation);

	void PaintTile();
	void EraseTile(class AGridMapStaticMeshActor* TileToErase);

	uint32 GetTileAdjacencyBitmask(class UWorld* World, const FVector& Origin, UGridMapTileSet* TileSet) const;
	bool TilesAt(class UWorld* World, const FVector& Origin, TArray<class AGridMapStaticMeshActor*>& OutTiles) const;
	bool GetAdjacentTiles(class UWorld* World, const FVector& Origin, TArray<TPair<class AGridMapStaticMeshActor*, uint32>>& OutAdjacentTiles, bool bIncludeEmptyTiles = false) const;
	void UpdateAdjacentTiles(class UWorld* World, const TArray<FAdjacentTile>& RootActors);

	FString CreateActorLabel(const class UGridMapTileSet* TileSet) const;

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
	UStaticMeshComponent* TileBrushComponent;
	/** The dynamic material of the tile brush. */
	class UMaterialInstanceDynamic* BrushMID;
	FColor BrushDefaultHighlightColor;
	FColor BrushWarningHighlightColor;
	FColor BrushCurrentHighlightColor;

	UPROPERTY()
	TArray<class UGridMapTileSet*> ActiveTileSets;
	
	UPROPERTY()
	class UGridMapTileSet* ActiveTileSet;
};
