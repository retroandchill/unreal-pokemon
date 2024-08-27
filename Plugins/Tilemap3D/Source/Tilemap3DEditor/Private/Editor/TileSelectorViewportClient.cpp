// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/TileSelectorViewportClient.h"
#include "AssetEditorModeManager.h"
#include "UnrealWidget.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Tileset/Tileset3D.h"

constexpr int32 TilesPerRow = 8;

FTileSelectorViewportClient::FTileSelectorViewportClient(UTileset3D *InTileSet) : Tileset(InTileSet) {
    ELevelViewportType NewViewportType = LVT_OrthoXY;
    SetViewModes(VMI_Unlit, VMI_Unlit);
    SetViewportType(NewViewportType);

    Widget->SetUsesEditorModeTools(ModeTools.Get());

    DrawHelper.bDrawGrid = false;
    DrawHelper.bDrawPivot = false;

    PreviewScene = &OwnedPreviewScene;
    static_cast<FAssetEditorModeManager *>(ModeTools.Get())->SetPreviewScene(PreviewScene);

    EngineShowFlags.DisableAdvancedFeatures();
    EngineShowFlags.SetCompositeEditorPrimitives(true);
    
    auto InitialLocation = FVector(0, 0, 0);
    auto InitialRotation = FRotator(0, 0, 0);
    SetInitialViewTransform(LVT_OrthoXY, 100.0f * InitialLocation, InitialRotation, DEFAULT_ORTHOZOOM);
}

void FTileSelectorViewportClient::AddReferencedObjects(FReferenceCollector &Collector) {
    FTilemap3DBaseEditorViewportClient::AddReferencedObjects(Collector);
    Collector.AddStableReferenceArray(&TileMeshes);
}

void FTileSelectorViewportClient::SetTileSet(UTileset3D *Tileset3D) {
    TileMeshes | UE::Ranges::ForEach(&USceneComponent::DestroyComponent, false);
    TileMeshes.Reset();
    Tileset = Tileset3D;
    if (Tileset3D == nullptr) {
        return;
    }
    
    int32 CurrentIndex = 0;
    auto TileSize = Tileset3D->GetTileSize();
    for (auto &Tile : Tileset3D->GetTiles()) {
        int32 X = CurrentIndex % TilesPerRow;
        int32 Y = CurrentIndex / TilesPerRow;

        auto TileComponent = NewObject<UStaticMeshComponent>();
        TileComponent->SetStaticMesh(Tile.TargetMesh.LoadSynchronous());
        auto TileTransform = Tile.TileRelativeTransform;
        TileTransform.SetLocation(TileTransform.GetLocation() + FVector(X * TileSize.X - TileSize.X * TilesPerRow / 2, Y * TileSize.Y, 0));
        PreviewScene->AddComponent(TileComponent, TileTransform);
        TileMeshes.Emplace(TileComponent);
        CurrentIndex += Tile.SizeX;
    }

    auto InitialLocation = FVector(0, 0, 0);
    auto InitialRotation = FRotator(0, 0, 0);
    SetInitialViewTransform(LVT_OrthoXY, 100.0f * InitialLocation, InitialRotation, DEFAULT_ORTHOZOOM / 2);
}