// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"

/**
 * Represents a single entry on the animation playback data
 */
struct FPaperZDWeightedAnimation
{
	/* AnimSequence being used. */
	TWeakObjectPtr<const UPaperZDAnimSequence> AnimSequencePtr;

	/* The playback marker. */
	float PlaybackTime;

	/* The weight that corresponds to the animation sequence. */
	float Weight;

	/* Animation layer. */
	int32 Layer;

	/* Layer weight. */
	float LayerWeight;

public:
	FPaperZDWeightedAnimation()
		: AnimSequencePtr(nullptr)
		, PlaybackTime(0.0f)
		, Weight(1.0f)
		, Layer(0)
		, LayerWeight(1.0f)
	{}

	//ctor
	FPaperZDWeightedAnimation(UPaperZDAnimSequence* InSequence, float InPlaybackTime = 0.0f, float InWeight = 1.0f)
		: AnimSequencePtr(InSequence)
		, PlaybackTime(InPlaybackTime)
		, Weight(InWeight)
		, Layer(0)
	{}
};

/**
 * Structure used to pass animation playback data from the nodes onto the final render block
 */
struct FPaperZDAnimationPlaybackData
{
	/* The list of weighted animations to apply. */
	TArray<FPaperZDWeightedAnimation> WeightedAnimations;

	/* The directional angle to use with multi-directional sequences. */
	float DirectionalAngle;

public:
	//ctor
	FPaperZDAnimationPlaybackData()
		: DirectionalAngle(0.0f)
	{}

	/* Overwrites the playback data to only hold the given animation. */
	void SetAnimation(const UPaperZDAnimSequence* Sequence, float PlaybackTime);

	/* Modifies this playback data so it only holds the animation with the greatest weight. */
	void RemoveWeights();
	
	/* Adds an animation with the given weight at the end of the stack. */
	void AddAnimation(const UPaperZDAnimSequence* Sequence, float PlaybackTime, float Weight = 1.0f);
};
