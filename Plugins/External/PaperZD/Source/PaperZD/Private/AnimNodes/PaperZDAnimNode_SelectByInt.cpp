// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_SelectByInt.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_SelectByInt)
#endif

FPaperZDAnimNode_SelectByInt::FPaperZDAnimNode_SelectByInt()
	: SelectValue(0)
	, bUpdateInactiveAnimations(false)
	, bResetOnChange(true)
	, OldSelectValue(INDEX_NONE)
{}

void FPaperZDAnimNode_SelectByInt::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	for (FPaperZDAnimDataLink& Anim : Animation)
	{
		Anim.Initialize(InitContext);
	}

	//Make sure both relevancy values are the same, so we don't trigger another initialize on the first tick
	OldSelectValue = SelectValue;
}

void FPaperZDAnimNode_SelectByInt::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	if (Animation.Num() > 0)
	{
		const int32 NewSelectedValue = GetSelectedValue();
		const bool bShouldResetOnChange = !bUpdateInactiveAnimations && bResetOnChange;
		const bool bHasChangedValue = OldSelectValue != NewSelectedValue;
		FPaperZDAnimDataLink& ActiveAnimation = Animation[NewSelectedValue];

		//First check if we should initialize the values
		if (bShouldResetOnChange && bHasChangedValue)
		{
			FPaperZDAnimationInitContext InitContext(UpdateContext.AnimInstance);
			ActiveAnimation.Initialize(InitContext);
		}

		//We should update the relevant animation
		ActiveAnimation.Update(UpdateContext);

		//Check if we should update the inactive animations
		if (bUpdateInactiveAnimations)
		{
			FPaperZDAnimationUpdateContext NoWeightContext = UpdateContext.FractionalWeight(0.0f);
			for (int32 i = 0; i < Animation.Num(); i++)
			{
				if (i != NewSelectedValue)
				{
					FPaperZDAnimDataLink& InactiveAnimation = Animation[i];
					InactiveAnimation.Update(NoWeightContext);
				}
			}
		}

		//Cache this value for next frame
		OldSelectValue = NewSelectedValue;
	}
}

void FPaperZDAnimNode_SelectByInt::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	if (Animation.Num() > 0)
	{
		const int32 ClampedValue = GetSelectedValue();
		FPaperZDAnimDataLink& ActiveAnimation = Animation[ClampedValue];
		ActiveAnimation.Evaluate(OutData);
	}
}

int32 FPaperZDAnimNode_SelectByInt::GetSelectedValue() const
{
	return FMath::Clamp<int32>(SelectValue, 0, Animation.Num() - 1);
}
