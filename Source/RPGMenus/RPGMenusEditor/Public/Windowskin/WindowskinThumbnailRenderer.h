// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "WindowskinThumbnailRenderer.generated.h"

/**
 * Class to handle the rendering of Windowskin thubmnails in the editor
 */
UCLASS()
class UWindowskinThumbnailRenderer : public UThumbnailRenderer {
	GENERATED_BODY()

public:
	bool AllowsRealtimeThumbnails(UObject* Object) const override;
	bool CanVisualizeAsset(UObject* Object) override;
	void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas,
	          bool bAdditionalViewFamily) override;
};
