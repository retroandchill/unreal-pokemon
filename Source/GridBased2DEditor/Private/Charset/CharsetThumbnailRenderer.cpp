// "Unreal Pokémon" created by Retro & Chill.
#include "Charset/CharsetThumbnailRenderer.h"

#include "Characters/Charset.h"
#include "PaperFlipbook.h"
#include "ThumbnailRendering/ThumbnailManager.h"

bool UCharsetThumbnailRenderer::AllowsRealtimeThumbnails(UObject* Object) const {
	return true;
}

bool UCharsetThumbnailRenderer::CanVisualizeAsset(UObject* Object) {
	auto Charset = Cast<UCharset>(Object);
	if (Charset == nullptr)
		return false;

	return Charset->GetDownSprite() != nullptr;
}

void UCharsetThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height,
	FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily) {
	auto Charset = Cast<UCharset>(Object);
	if (Charset == nullptr)
		return;

	auto DownSprite = Charset->GetDownSprite();
	if (DownSprite == nullptr)
		return;

	auto ThumbnailInfo = UThumbnailManager::Get().GetRenderingInfo(DownSprite);
	if (ThumbnailInfo == nullptr || ThumbnailInfo->Renderer == nullptr)
		return;
	
	ThumbnailInfo->Renderer->Draw(DownSprite, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}
