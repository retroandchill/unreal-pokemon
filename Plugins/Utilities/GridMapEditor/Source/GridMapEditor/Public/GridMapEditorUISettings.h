#pragma once

#include "CoreMinimal.h"
#include "GridMapEditorTypes.h"
#include "TileSet.h"

struct GRIDMAPEDITOR_API FGridMapEditorUISettings
{
    FGridMapEditorUISettings() = default;

    // tool
    bool GetPaintToolSelected() const
    {
        return bPaintToolSelected;
    }

    void SetPaintToolSelected(bool InbPaintToolSelected)
    {
        bPaintToolSelected = InbPaintToolSelected;
    }

    bool GetSelectToolSelected() const
    {
        return bSelectToolSelected;
    }

    void SetSelectToolSelected(bool bInSelectToolSelected)
    {
        bSelectToolSelected = bInSelectToolSelected;
    }

    bool GetSettingsToolSelected() const
    {
        return bSettingsToolSelected;
    }

    void SetSettingsToolSelected(bool bInSettingsToolSelected)
    {
        bSettingsToolSelected = bInSettingsToolSelected;
    }

    FVector GetPaintOrigin() const
    {
        return PaintOrigin;
    }

    void SetPaintOrigin(const FVector &NewPaintOrigin)
    {
        PaintOrigin = NewPaintOrigin;
    }

    float GetPaintHeight() const
    {
        return PaintHeight;
    }

    void SetPaintHeight(float InPaintHeight)
    {
        PaintHeight = InPaintHeight;
    }

    int32 GetPaintLayer() const
    {
        return PaintLayer;
    }

    void SetPaintLayer(int32 InPaintLayer)
    {
        PaintLayer = InPaintLayer;
    }

    EGridMapPaintMode GetPaintMode() const
    {
        return PaintMode;
    }

    void SetPaintMode(EGridMapPaintMode InPaintMode)
    {
        PaintMode = InPaintMode;
    }

    TWeakObjectPtr<class UGridMapTileSet> GetCurrentTileSet() const
    {
        return CurrentTileSetPtr;
    }

    void SetCurrentTileSet(class UGridMapTileSet *NewTileSet)
    {
        CurrentTileSetPtr = NewTileSet;
    }

    bool GetDebugDrawTiles() const
    {
        return bDebugDrawUpdatedTiles;
    }

    void SetDebugDrawTiles(bool bInDebugDrawUpdatedTiles)
    {
        bDebugDrawUpdatedTiles = bInDebugDrawUpdatedTiles;
    }

  private:
    bool bPaintToolSelected = true;
    bool bSelectToolSelected = false;
    bool bSettingsToolSelected = false;
    float PaintHeight = 0.f;
    int32 PaintLayer = 0;

    FVector PaintOrigin = FVector::ZeroVector;
    EGridMapPaintMode PaintMode = EGridMapPaintMode::Paint;

    bool bDebugDrawUpdatedTiles = false;

    TWeakObjectPtr<UGridMapTileSet> CurrentTileSetPtr;
};