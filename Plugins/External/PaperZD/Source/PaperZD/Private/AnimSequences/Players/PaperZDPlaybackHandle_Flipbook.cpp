// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimSequences/Players/PaperZDPlaybackHandle_Flipbook.h"
#include "AnimSequences/Players/PaperZDAnimationPlaybackData.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDPlaybackHandle_Flipbook)
#endif

void UPaperZDPlaybackHandle_Flipbook::UpdateRenderPlayback(UPrimitiveComponent* RenderComponent, const FPaperZDAnimationPlaybackData& PlaybackData, bool bIsPreviewPlayback /* = false */)
{
	UPaperFlipbookComponent* Sprite = Cast<UPaperFlipbookComponent>(RenderComponent);
	if (Sprite)
	{
		const FPaperZDWeightedAnimation& PrimaryAnimation = PlaybackData.WeightedAnimations[0];
		UPaperFlipbook* Flipbook = PrimaryAnimation.AnimSequencePtr->GetAnimationData<UPaperFlipbook*>(PlaybackData.DirectionalAngle, bIsPreviewPlayback);

		//Check if the flipbook hasn't changed
		if (Sprite->GetFlipbook() != Flipbook)
		{
			UPaperFlipbook* From = Sprite->GetFlipbook();
			Sprite->SetFlipbook(Flipbook);
		}

		//We manage the time manually
		Sprite->SetPlaybackPosition(PrimaryAnimation.PlaybackTime, false);
	}
}

void UPaperZDPlaybackHandle_Flipbook::ConfigureRenderComponent(UPrimitiveComponent* RenderComponent, bool bIsPreviewPlayback /* = false */)
{
	//Stop the flipbook from ticking itself, the playback is managed by the player now
	UPaperFlipbookComponent* Sprite = Cast<UPaperFlipbookComponent>(RenderComponent);
	if (Sprite)
	{
		Sprite->Stop();
		Sprite->SetLooping(false);
	}
}
