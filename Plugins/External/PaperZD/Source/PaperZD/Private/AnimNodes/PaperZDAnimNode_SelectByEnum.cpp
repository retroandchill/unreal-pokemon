// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_SelectByEnum.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_SelectByEnum)
#endif

FPaperZDAnimNode_SelectByEnum::FPaperZDAnimNode_SelectByEnum()
	: SelectValue(0)
	, bUpdateInactiveAnimations(false)
	, bResetOnChange(true)
	, OldAnimationIndex(INDEX_NONE)
{}

void FPaperZDAnimNode_SelectByEnum::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	for (FPaperZDAnimDataLink& Anim : Animation)
	{
		Anim.Initialize(InitContext);
	}

	//Make sure both relevancy values are the same, so we don't trigger another initialize on the first tick
	OldAnimationIndex = GetSelectedAnimationIndex();
}

void FPaperZDAnimNode_SelectByEnum::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	if (Animation.Num() > 0)
	{
		const int32 AnimationIndex = GetSelectedAnimationIndex();
		const bool bShouldResetOnChange = !bUpdateInactiveAnimations && bResetOnChange;
		const bool bHasChangedValue = OldAnimationIndex != AnimationIndex;
		FPaperZDAnimDataLink& ActiveAnimation = Animation[AnimationIndex];

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
				if (i != AnimationIndex)
				{
					FPaperZDAnimDataLink& InactiveAnimation = Animation[i];
					InactiveAnimation.Update(NoWeightContext);
				}
			}
		}

		//Cache this value for next frame
		OldAnimationIndex = AnimationIndex;
	}
}

void FPaperZDAnimNode_SelectByEnum::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	if (Animation.Num() > 0)
	{
		const int32 AnimationIndex = GetSelectedAnimationIndex();
		FPaperZDAnimDataLink& ActiveAnimation = Animation[AnimationIndex];
		ActiveAnimation.Evaluate(OutData);
	}
}

int32 FPaperZDAnimNode_SelectByEnum::GetSelectedAnimationIndex() const
{
	//Translate enum value to animation value and send back, if for any reason this fails, send a 'default' value (index 0)
	const int32 EnumIndex = static_cast<int32>(SelectValue);
	return EnumToAnimationIndex.IsValidIndex(EnumIndex) ? EnumToAnimationIndex[EnumIndex] : 0;
}
