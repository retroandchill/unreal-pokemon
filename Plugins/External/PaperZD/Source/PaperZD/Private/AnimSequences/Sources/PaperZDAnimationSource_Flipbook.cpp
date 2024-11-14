// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimSequences/Sources/PaperZDAnimationSource_Flipbook.h"
#include "AnimSequences/Players/PaperZDPlaybackHandle_Flipbook.h"
#include "AnimSequences/PaperZDAnimSequence_Flipbook.h"
#include "PaperFlipbookComponent.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimationSource_Flipbook)
#endif

UPaperZDAnimationSource_Flipbook::UPaperZDAnimationSource_Flipbook()
{
	SupportedAnimSequenceClass = UPaperZDAnimSequence_Flipbook::StaticClass();
	bSupportsBlending = false;
	bSupportsAnimationLayers = false;
}

TSubclassOf<UPaperZDPlaybackHandle> UPaperZDAnimationSource_Flipbook::GetPlaybackHandleClass() const
{
	return UPaperZDPlaybackHandle_Flipbook::StaticClass();
}

TSubclassOf<UPrimitiveComponent> UPaperZDAnimationSource_Flipbook::GetRenderComponentClass() const
{
	return UPaperFlipbookComponent::StaticClass();
}
