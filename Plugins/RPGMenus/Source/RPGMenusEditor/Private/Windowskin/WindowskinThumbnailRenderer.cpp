// "Unreal Pokémon" created by Retro & Chill.
#include "Windowskin/WindowskinThumbnailRenderer.h"
#include "Data/Windowskin.h"
#include "ThumbnailRendering/ThumbnailManager.h"

EThumbnailRenderFrequency UWindowskinThumbnailRenderer::GetThumbnailRenderFrequency(UObject *Object) const
{
    return EThumbnailRenderFrequency::Realtime;
}

bool UWindowskinThumbnailRenderer::CanVisualizeAsset(UObject *Object)
{
    auto Windowskin = Cast<UWindowskin>(Object);
    if (Windowskin == nullptr)
        return false;

    return Windowskin->GetSourceTexture() != nullptr;
}

void UWindowskinThumbnailRenderer::Draw(UObject *Object, int32 X, int32 Y, uint32 Width, uint32 Height,
                                        FRenderTarget *RenderTarget, FCanvas *Canvas, bool bAdditionalViewFamily)
{
    auto Windowskin = Cast<UWindowskin>(Object);
    if (Windowskin == nullptr)
        return;

    auto SourceTexture = Windowskin->GetSourceTexture();
    if (SourceTexture == nullptr)
        return;

    auto ThumbnailInfo = UThumbnailManager::Get().GetRenderingInfo(SourceTexture);
    if (ThumbnailInfo == nullptr || ThumbnailInfo->Renderer == nullptr)
        return;

    ThumbnailInfo->Renderer->Draw(SourceTexture, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}
