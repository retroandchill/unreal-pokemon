#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ThumbnailRendering/StaticMeshThumbnailRenderer.h"
#include "TileSet_ThumbnailRenderer.generated.h"

class FCanvas;
class FRenderTarget;

UCLASS(Config = Editor)
class UTileSet_ThumbnailRenderer : public UStaticMeshThumbnailRenderer
{
	GENERATED_BODY()
	
	// UThumbnailRenderer implementation
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas, bool bAdditionalViewFamily) override;

};
