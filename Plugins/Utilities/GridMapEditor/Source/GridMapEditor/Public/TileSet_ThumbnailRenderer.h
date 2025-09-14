#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/StaticMeshThumbnailRenderer.h"
#include "TileMeshThumbnailScene.h"
#include "UObject/ObjectMacros.h"

#include "TileSet_ThumbnailRenderer.generated.h"

class FCanvas;
class FRenderTarget;

UCLASS(Config = Editor)
class GRIDMAPEDITOR_API UTileSet_ThumbnailRenderer : public UDefaultSizedThumbnailRenderer
{
    GENERATED_BODY()

  public:
    void Draw(UObject *Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget *RenderTarget,
              FCanvas *Canvas, bool bAdditionalViewFamily) override;
    void BeginDestroy() override;

  private:
    TUniquePtr<FTileMeshThumbnailScene> ThumbnailScene;
};
