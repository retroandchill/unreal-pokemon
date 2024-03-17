// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "CharsetThumbnailRenderer.generated.h"

/**
 * Class to handle the rendering of Charset thubmnails in the editor
 */
UCLASS()
class GRIDBASED2DEDITOR_API UCharsetThumbnailRenderer : public UThumbnailRenderer {
	GENERATED_BODY()

public:
	EThumbnailRenderFrequency GetThumbnailRenderFrequency(UObject* Object) const override;
	bool CanVisualizeAsset(UObject* Object) override;
	void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas,
	          bool bAdditionalViewFamily) override;
};
