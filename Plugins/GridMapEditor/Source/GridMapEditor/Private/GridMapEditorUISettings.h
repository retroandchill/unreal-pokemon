#pragma once

#include "CoreMinimal.h"
#include "GridMapEditorTypes.h"
#include "TileSet.h"

struct FGridMapEditorUISettings
{
public:
	FGridMapEditorUISettings();

	// tool
	bool GetPaintToolSelected() const { return bPaintToolSelected ? true : false; }
	void SetPaintToolSelected(bool InbPaintToolSelected) { bPaintToolSelected = InbPaintToolSelected; }

	bool GetSelectToolSelected() const { return bSelectToolSelected ? true : false; }
	void SetSelectToolSelected(bool bInSelectToolSelected) { bSelectToolSelected = bInSelectToolSelected; }

	bool GetSettingsToolSelected() const { return bSettingsToolSelected; }
	void SetSettingsToolSelected(bool bInSettingsToolSelected) { bSettingsToolSelected = bInSettingsToolSelected; }

	FVector GetPaintOrigin() const { return PaintOrigin; }
	void SetPaintOrigin(const FVector& NewPaintOrigin) { PaintOrigin = NewPaintOrigin; }

	EGridMapPaintMode GetPaintMode() const { return PaintMode; }
	void SetPaintMode(EGridMapPaintMode InPaintMode) { PaintMode = InPaintMode; }	

	bool GetHideOwnedActors() const { return bHideOwnedActors; }
	void SetHideOwnedActors(bool bInHideActors) { bHideOwnedActors = bInHideActors; }

	TWeakObjectPtr<class UGridMapTileSet> GetCurrentTileSet() const { return CurrentTileSetPtr; }
	void SetCurrentTileSet(class UGridMapTileSet* NewTileSet) { CurrentTileSetPtr = NewTileSet; }

	bool GetDebugDrawTiles() const { return bDebugDrawUpdatedTiles; }
	void SetDebugDrawTiles(bool bInDebugDrawUpdatedTiles) { bDebugDrawUpdatedTiles = bInDebugDrawUpdatedTiles; }

private:
	bool bPaintToolSelected;
	bool bSelectToolSelected;
	bool bSettingsToolSelected;

	FVector PaintOrigin;
	EGridMapPaintMode PaintMode;
	bool bHideOwnedActors;

	bool bDebugDrawUpdatedTiles;

	TWeakObjectPtr<class UGridMapTileSet> CurrentTileSetPtr;
	
};
