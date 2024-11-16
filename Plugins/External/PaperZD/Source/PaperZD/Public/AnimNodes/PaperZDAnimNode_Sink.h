// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_Sink.generated.h"

/**
 * Sink runtime node that collects the final animation data
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_Sink : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Resulting Animation Data. */
	UPROPERTY(EditAnywhere, Category = "Input")
	FPaperZDAnimDataLink Result;

	/* The name of this sink node, used to identify the output of this graph. Filled in by the compiler, propagated from the parent graph. */
	UPROPERTY()
	FName Name;

public:
	//ctor
	FPaperZDAnimNode_Sink();

	/* True if this is the main "sink" node that outputs the final "animation data". */
	bool IsMainSinkNode() const;

	/* True if this sink node has data connected to it, otherwise it was ignored while compiling. */
	bool HasAnimationData() const;

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface

};
