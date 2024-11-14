// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_SetDirectionality.generated.h"

/**
 * Sets the directionality data for multi-directional sequence rendering.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_SetDirectionality : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Animation data. */
	UPROPERTY(EditAnywhere, Category = "Input")
	FPaperZDAnimDataLink Animation;

	/* The point in the 2D map of the multi-directional sequence which will be evaluated to obtain the resulting animation. */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AlwaysAsPin))
	FVector2D Input;

	/* The directional angle, already cached. */
	float CachedDirectionalAngle;

public:
	//ctor
	FPaperZDAnimNode_SetDirectionality();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface

};
