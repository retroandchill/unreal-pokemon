// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimSequences/Players/PaperZDAnimationPlaybackData.h"

void FPaperZDAnimationPlaybackData::SetAnimation(const UPaperZDAnimSequence* Sequence, float PlaybackTime)
{
	WeightedAnimations.Empty(1);
	FPaperZDWeightedAnimation& AnimWeight = WeightedAnimations.AddDefaulted_GetRef();
	AnimWeight.AnimSequencePtr = Sequence;
	AnimWeight.PlaybackTime = PlaybackTime;
}

void FPaperZDAnimationPlaybackData::RemoveWeights()
{
	//Find the animation with the biggest weight and erase any other
	int32 BiggestWeightIndex = INDEX_NONE;
	float BiggestWeight = -1.0f;
	for (int32 i = 0; i < WeightedAnimations.Num(); i++)
	{
		if (WeightedAnimations[i].Weight > BiggestWeight)
		{
			BiggestWeightIndex = i;
		}
	}

	//Overwrite the animations with the biggest one
	if (BiggestWeightIndex != INDEX_NONE)
	{
		SetAnimation(WeightedAnimations[BiggestWeightIndex].AnimSequencePtr.Get(), WeightedAnimations[BiggestWeightIndex].PlaybackTime);
	}
}

void FPaperZDAnimationPlaybackData::AddAnimation(const UPaperZDAnimSequence* Sequence, float PlaybackTime, float Weight /* = 1.0f */)
{
	if (Weight > 0.0f)
	{
		FPaperZDWeightedAnimation& AnimWeight = WeightedAnimations.AddDefaulted_GetRef();
		AnimWeight.AnimSequencePtr = Sequence;
		AnimWeight.PlaybackTime = PlaybackTime;
		AnimWeight.Weight = Weight;
	}
}
