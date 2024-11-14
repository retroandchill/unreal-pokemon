// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_SelectByBool.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_SelectByBool)
#endif

FPaperZDAnimNode_SelectByBool::FPaperZDAnimNode_SelectByBool()
	: bSelectValue(false)
	, bUpdateInactiveAnimation(false)
	, bResetOnChange(true)
	, bOldSelectValue(false)
{}

void FPaperZDAnimNode_SelectByBool::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	TrueAnimation.Initialize(InitContext);
	FalseAnimation.Initialize(InitContext);

	//Make sure both relevancy values are the same, so we don't trigger another initialize on the first tick
	bOldSelectValue = bSelectValue;
}

void FPaperZDAnimNode_SelectByBool::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{	
	const bool bShouldResetOnChange = !bUpdateInactiveAnimation && bResetOnChange;
	const bool bHasChangedValue = bOldSelectValue != bSelectValue;
	FPaperZDAnimDataLink& ActiveAnimation = bSelectValue ? TrueAnimation : FalseAnimation;
	FPaperZDAnimDataLink& InactiveAnimation = bSelectValue ? FalseAnimation : TrueAnimation;

	//First check if we should initialize the values
	if (bShouldResetOnChange && bHasChangedValue)
	{
		FPaperZDAnimationInitContext InitContext(UpdateContext.AnimInstance);
		ActiveAnimation.Initialize(InitContext);
	}

	//We should update the relevant animation
	ActiveAnimation.Update(UpdateContext);

	//Check if we should update the inactive animation
	if (bUpdateInactiveAnimation)
	{
		InactiveAnimation.Update(UpdateContext.FractionalWeight(0.0f));
	}

	//Cache this value for next frame
	bOldSelectValue = bSelectValue;
}

void FPaperZDAnimNode_SelectByBool::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	FPaperZDAnimDataLink& ActiveAnimation = bSelectValue ? TrueAnimation : FalseAnimation;
	ActiveAnimation.Evaluate(OutData);
}
