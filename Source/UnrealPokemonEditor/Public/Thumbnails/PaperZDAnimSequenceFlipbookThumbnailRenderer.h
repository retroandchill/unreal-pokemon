// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/DefaultSizedThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "PaperZDAnimSequenceFlipbookThumbnailRenderer.generated.h"

class UPaperZDAnimSequence_Flipbook;
class UPaperFlipbook;
/**
 * 
 */
UCLASS(Config = Editor)
class UNREALPOKEMONEDITOR_API UPaperZDAnimSequenceFlipbookThumbnailRenderer : public UDefaultSizedThumbnailRenderer {
    GENERATED_BODY()

public:
    void Draw(UObject *Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget *, FCanvas *Canvas,
              bool bAdditionalViewFamily) override;

};
