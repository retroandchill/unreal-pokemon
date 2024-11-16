// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_CacheAnimation.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_CacheAnimation)
#endif

FPaperZDAnimNode_CacheAnimation::FPaperZDAnimNode_CacheAnimation()
	: LastUpdateFrameNumber(INDEX_NONE)
	, bEverInitialized(false)
	, bStaleAnimationData(true)
{}

void FPaperZDAnimNode_CacheAnimation::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	//Will only initialize once, independent if called again (which will happen if any state machine node uses this cache).
	if (!bEverInitialized)
	{
		Animation.Initialize(InitContext);
		bEverInitialized = true;

		//Make sure the next update triggers, by forcibly setting the frame number to be stale.
		LastUpdateFrameNumber = GFrameNumber - 1; 
	}
}

void FPaperZDAnimNode_CacheAnimation::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	//We only update once per frame
	if (LastUpdateFrameNumber != GFrameNumber)
	{
		Animation.Update(UpdateContext);
		LastUpdateFrameNumber = GFrameNumber;
		bStaleAnimationData = true;
	}
}

void FPaperZDAnimNode_CacheAnimation::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	if (bStaleAnimationData)
	{
		Animation.Evaluate(OutData);
		CachedAnimationData = OutData;
		bStaleAnimationData = false;
	}
	else
	{
		OutData = CachedAnimationData;
	}
}
