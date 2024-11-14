// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_UseCachedAnimation.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_UseCachedAnimation)
#endif

FPaperZDAnimNode_UseCachedAnimation::FPaperZDAnimNode_UseCachedAnimation()
{}

void FPaperZDAnimNode_UseCachedAnimation::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	LinkedCacheNode.Initialize(InitContext);
}

void FPaperZDAnimNode_UseCachedAnimation::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	LinkedCacheNode.Update(UpdateContext);
}

void FPaperZDAnimNode_UseCachedAnimation::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	LinkedCacheNode.Evaluate(OutData);
}
