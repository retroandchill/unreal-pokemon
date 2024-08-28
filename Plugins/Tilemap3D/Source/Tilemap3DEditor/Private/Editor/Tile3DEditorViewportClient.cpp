// "Unreal Pokémon" created by Retro & Chill.

#include "Editor/Tile3DEditorViewportClient.h"
#include "AssetEditorModeManager.h"
#include "CanvasTypes.h"
#include "Tileset/Tileset3D.h"
#include "UnrealWidget.h"

FTile3DEditorViewportClient::FTile3DEditorViewportClient(UTileset3D *InTileSet) : TileSet(InTileSet) {
    SetRealtime(true);

    // The tile map editor fully supports mode tools and isn't doing any incompatible stuff with the Widget
    Widget->SetUsesEditorModeTools(ModeTools.Get());

    DrawHelper.bDrawGrid = true;
    DrawHelper.bDrawPivot = false;

    PreviewScene = &OwnedPreviewScene;
    static_cast<FAssetEditorModeManager *>(ModeTools.Get())->SetPreviewScene(PreviewScene);

    EngineShowFlags.DisableAdvancedFeatures();
    EngineShowFlags.SetCompositeEditorPrimitives(true);

    // Create a render component for the tile preview
    PreviewTileSpriteComponent = NewObject<UStaticMeshComponent>();
    PreviewScene->AddComponent(PreviewTileSpriteComponent, FTransform::Identity);

    // Get the correct general direction of the perspective mode; the distance doesn't matter much as we've queued up a
    // deferred zoom that will calculate a much better distance
    auto InitialLocation = FVector(0, 1, 1);
    auto InitialRotation = FRotator(-45, -90, 0);
    SetInitialViewTransform(LVT_Perspective, 100.0f * InitialLocation, InitialRotation, DEFAULT_ORTHOZOOM);
}

void FTile3DEditorViewportClient::Tick(float DeltaSeconds) {
    FEditorViewportClient::Tick(DeltaSeconds);
    OwnedPreviewScene.GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
}

FLinearColor FTile3DEditorViewportClient::GetBackgroundColor() const {
    return FLinearColor::Blue;
}

void FTile3DEditorViewportClient::DrawCanvas(FViewport &InViewport, FSceneView &View, FCanvas &Canvas) {
    if (const bool bIsHitTesting = Canvas.IsHitTesting(); !bIsHitTesting) {
        Canvas.SetHitProxy(nullptr);
    }
    FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);
}

void FTile3DEditorViewportClient::SetTileSet(UTileset3D *Tileset3D) {
    TileSet = Tileset3D;
    SetTileIndex(INDEX_NONE);
}

void FTile3DEditorViewportClient::SetTileIndex(int32 InTileIndex) {
    const int32 OldTileIndex = TileBeingEditedIndex;
    auto &Tiles = TileSet->GetTiles();
    const bool bNewIndexValid = (InTileIndex >= 0) && (InTileIndex < Tiles.Num());
    TileBeingEditedIndex = bNewIndexValid ? InTileIndex : INDEX_NONE;

    // Update the visual representation
    if (TileBeingEditedIndex != INDEX_NONE && Tiles.IsValidIndex(TileBeingEditedIndex)) {
        auto &Tile = Tiles[TileBeingEditedIndex];
        PreviewTileSpriteComponent->SetStaticMesh(Tile.TargetMesh.LoadSynchronous());
        PreviewTileSpriteComponent->SetWorldTransform(Tile.TileRelativeTransform);
    } else {
        PreviewTileSpriteComponent->SetStaticMesh(nullptr);
    }

    // Zoom to fit when we start editing a tile and weren't before, but leave it alone if we just changed tiles, in case
    // they zoomed in or out further
    if ((TileBeingEditedIndex != INDEX_NONE) && (OldTileIndex == INDEX_NONE)) {
        RequestFocusOnSelection(/*bInstant=*/true);
    }

    // Trigger a details panel customization rebuild
    OnSingleTileIndexChanged.Broadcast(TileBeingEditedIndex, OldTileIndex);

    // Redraw the viewport
    Invalidate();
    Invalidate();
}

int32 FTile3DEditorViewportClient::GetTileIndex() const {
    return TileBeingEditedIndex;
}

FBox FTile3DEditorViewportClient::GetDesiredFocusBounds() const {
    return PreviewTileSpriteComponent->Bounds.GetBox();
}