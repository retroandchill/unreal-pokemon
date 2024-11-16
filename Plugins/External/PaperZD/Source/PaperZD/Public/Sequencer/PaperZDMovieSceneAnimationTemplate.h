// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "Sequencer/PaperZDMovieSceneAnimationSection.h"
#include "PaperZDMovieSceneAnimationTemplate.generated.h"

USTRUCT()
struct FPaperZDMovieSceneAnimationSectionTemplateParams : public FPaperZDMovieSceneAnimationParams
{
	GENERATED_BODY()

	/* Start time of the section. */
	UPROPERTY()
	FFrameNumber SectionStartTime;

	/* End time of the section. */
	UPROPERTY()
	FFrameNumber SectionEndTime;

public:

	//ctors
	FPaperZDMovieSceneAnimationSectionTemplateParams() {}

	FPaperZDMovieSceneAnimationSectionTemplateParams(const FPaperZDMovieSceneAnimationParams& BaseParams, FFrameNumber InSectionStartTime, FFrameNumber InSectionEndTime)
		: FPaperZDMovieSceneAnimationParams(BaseParams)
		, SectionStartTime(InSectionStartTime)
		, SectionEndTime(InSectionEndTime)
	{}

	float MapTimeToAnimation(FFrameTime InPosition, FFrameRate InFrameRate, class UPaperZDAnimSequence* Sequence) const;
};

USTRUCT()
struct FPaperZDMovieSceneAnimationTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	/* Params that gobern this template. */
	UPROPERTY()
	FPaperZDMovieSceneAnimationSectionTemplateParams Params;

	//ctors
	FPaperZDMovieSceneAnimationTemplate() {}	
	FPaperZDMovieSceneAnimationTemplate(const UPaperZDMovieSceneAnimationSection& Section);

	//Required
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }

	/* Called to evaluate the template into an animation pose. */
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
};
