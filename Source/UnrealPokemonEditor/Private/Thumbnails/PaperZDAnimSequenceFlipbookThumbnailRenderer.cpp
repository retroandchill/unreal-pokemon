// "Unreal Pokémon" created by Retro & Chill.

#include "Thumbnails/PaperZDAnimSequenceFlipbookThumbnailRenderer.h"
#include "AnimSequences/PaperZDAnimSequence_Flipbook.h"
#include "ThumbnailRendering/ThumbnailManager.h"

constexpr float DOWN_ANGLE = 180.f;

void UPaperZDAnimSequenceFlipbookThumbnailRenderer::Draw(UObject *Object, int32 X, int32 Y, uint32 Width, uint32 Height,
                                                         FRenderTarget *RenderTarget, FCanvas *Canvas,
                                                         bool bAdditionalViewFamily) {
    auto Sequence = Cast<UPaperZDAnimSequence_Flipbook>(Object);
    if (Sequence->GetAnimDataSourceProperty() == nullptr) {
        return;
    }

    auto Flipbook = Sequence->GetAnimationData<UPaperFlipbook *>(DOWN_ANGLE);
    if (Flipbook == nullptr)
        return;

    auto ThumbnailInfo = UThumbnailManager::Get().GetRenderingInfo(Flipbook);
    if (ThumbnailInfo == nullptr || ThumbnailInfo->Renderer == nullptr)
        return;

    ThumbnailInfo->Renderer->Draw(Flipbook, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}