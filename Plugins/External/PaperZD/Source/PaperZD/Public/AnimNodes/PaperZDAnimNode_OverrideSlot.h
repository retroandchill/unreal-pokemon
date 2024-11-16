// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_OverrideSlot.generated.h"

/**
 * Creates an entry point for animations to be played through code that will override the AnimationGraph.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_OverrideSlot : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Animation entry. */
	UPROPERTY(EditAnywhere, Category = "Input")
	FPaperZDAnimDataLink Animation;

	/* Name of the slot which will be called through code. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (NeverAsPin))
	FName SlotName;

	/* Group that this slot belongs to. Only one override can be played per group and any new animation played will cancel every override on this group. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (NeverAsPin))
	FName GroupName;

private:
	/* If there's an override currently playing for this slot. */
	bool bOverride;

	/* The playback data that we're gonna override the slot with. */
	FPaperZDAnimationPlaybackData OverrideData;

public:
	//ctor
	FPaperZDAnimNode_OverrideSlot();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface
};
