// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_PlaySequence.generated.h"

class UPaperZDAnimSequence;

/**
 * Plays a given Animation Sequence.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_PlaySequence : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Resulting Animation Data. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (HidePinByDefault))
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	/* The PlayRate multiplier against the normal speed of the AnimSequence. Can be negative, which will result on the animation going in reverse. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (HidePinByDefault))
	float PlayRate;

	/* The starting position of the animation when initializing. After the animation loops, it will start from 0. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (HidePinByDefault))
	float StartPosition;

	/* If the animation should loop. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (HidePinByDefault, PropertyFeature = "NonTransitional"))
	bool bLoopAnimation;

	/* The internal playback time. */
	float PlaybackTime;

public:
	//ctor
	FPaperZDAnimNode_PlaySequence();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface

	/* Obtain the AnimSequence bound to this play node. */
	UPaperZDAnimSequence* GetAnimSequence() const { return AnimSequence; }

	/* Obtain the current playback marker time. */
	float GetPlaybackTime() const { return PlaybackTime; }
};
