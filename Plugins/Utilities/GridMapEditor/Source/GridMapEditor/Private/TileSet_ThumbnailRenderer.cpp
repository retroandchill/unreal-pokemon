#include "TileSet_ThumbnailRenderer.h"
#include "Engine/StaticMesh.h"
#include "Misc/App.h"
#include "SceneView.h"
#include "ShowFlags.h"
#include "TileMeshThumbnailScene.h"
#include "TileSet.h"

void UTileSet_ThumbnailRenderer::Draw(UObject *Object, int32 X, int32 Y, uint32 Width, uint32 Height,
                                      FRenderTarget *RenderTarget, FCanvas *Canvas, bool bAdditionalViewFamily) {
    UGridMapTileSet *TileSet = Cast<UGridMapTileSet>(Object);

    // invalid or empty
    if (TileSet == nullptr || TileSet->Tiles.Num() == 0)
        return;

    FGridMapTileList *TileList = nullptr;
    for (FGridMapTileList &CheckTileList : TileSet->Tiles) {
        if (CheckTileList.Tiles.Num() > 0) {
            if (TileList == nullptr)
                TileList = &CheckTileList;

            if (CheckTileList.TileAdjacency.Bitset == 0) {
                TileList = &CheckTileList;
                break;
            }
        }
    }

    if (TileList == nullptr)
        return;

    TileSet->MeshForIcon = TileList->Tiles[0].LoadSynchronous();
    if (!TileSet->MeshForIcon) {
        return;
    }

    if (ThumbnailScene == nullptr || !ensure(ThumbnailScene->GetWorld() != nullptr)) {
        if (ThumbnailScene) {
            FlushRenderingCommands();
        }
        ThumbnailScene = MakeUnique<FTileMeshThumbnailScene>();
    }

    ThumbnailScene->SetStaticMesh(TileSet->MeshForIcon);
    ThumbnailScene->SetActorRotation(TileList->Rotation);
    ThumbnailScene->GetScene()->UpdateSpeedTreeWind(0.0);

    FSceneViewFamilyContext ViewFamily(
        FSceneViewFamily::ConstructionValues(RenderTarget, ThumbnailScene->GetScene(), FEngineShowFlags(ESFIM_Game))
            .SetTime(UThumbnailRenderer::GetTime())
            .SetAdditionalViewFamily(bAdditionalViewFamily));

    ViewFamily.EngineShowFlags.DisableAdvancedFeatures();
    ViewFamily.EngineShowFlags.MotionBlur = 0;
    ViewFamily.EngineShowFlags.LOD = 0;

    RenderViewFamily(Canvas, &ViewFamily, ThumbnailScene->CreateView(&ViewFamily, X, Y, Width, Height));
    ThumbnailScene->SetStaticMesh(nullptr);
}

void UTileSet_ThumbnailRenderer::BeginDestroy() {
    Super::BeginDestroy();
    ThumbnailScene.Reset();
}
