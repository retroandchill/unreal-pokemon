// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_UseCachedAnimation.generated.h"

/**
 * Reads from a cached animation node.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_UseCachedAnimation : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Link to the CacheAnimation data link. */
	UPROPERTY()
	FPaperZDAnimDataLink LinkedCacheNode;

public:
	//ctor
	FPaperZDAnimNode_UseCachedAnimation();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface

};
