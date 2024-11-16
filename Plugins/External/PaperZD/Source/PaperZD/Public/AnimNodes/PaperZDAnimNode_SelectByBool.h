// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_SelectByBool.generated.h"

/**
 * Selects between two animations depending on a given boolean value.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_SelectByBool : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* The value that drives the selection. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (AlwaysAsPin))
	bool bSelectValue;

	/**
	 * If true, the inactive animation will still update its playback progress as if its still running.
	 * Switching between animations will not reset them and they will just take from where they were.
	 * Disabling this pauses animations update and the inactive animation will be "frozen in time" until selected again.
	 */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (NeverAsPin))
	bool bUpdateInactiveAnimation;
		
	/* If animation aren't updating while inactive this value will force them to "reset" to 0 when they become relevant again. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (NeverAsPin, EditCondition = "!bUpdateInactiveAnimation"))
	bool bResetOnChange;

	/* Animation to show when the select value is true. */
	UPROPERTY(EditAnywhere, Category = "Input")
	FPaperZDAnimDataLink TrueAnimation;

	/* Animation to show when the select value is false. */
	UPROPERTY(EditAnywhere, Category = "Input")
	FPaperZDAnimDataLink FalseAnimation;

	/* Old relevancy value. */
	bool bOldSelectValue;

public:
	//ctor
	FPaperZDAnimNode_SelectByBool();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface
};
