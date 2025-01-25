// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "SimpleFlipbookThumbnailRenderer.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLE2DEDITOR_API USimpleFlipbookThumbnailRenderer : public UThumbnailRenderer {
    GENERATED_BODY()

public:
    void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas, bool bAdditionalViewFamily) override;

private:
    static void DrawFrame(USimpleFlipbook* Flipbook, int32 Index, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas);
    static void DrawGrid(int32 X, int32 Y, uint32 Width, uint32 Height, FCanvas* Canvas);
};
