// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_SelectByInt.generated.h"

/**
 * Selects between a list of animations depending on a given integer value.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_SelectByInt : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* The value that drives the selection. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (AlwaysAsPin))
	int32 SelectValue;

	/**
	 * If true, the inactive animations will still update its playback progress as if its still running.
	 * Switching between animations will not reset them and they will just take from where they were.
	 * Disabling this pauses animations update and the inactive animations will be "frozen in time" until selected again.
	 */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (NeverAsPin))
	bool bUpdateInactiveAnimations;
		
	/* If animation aren't updating while inactive this value will force them to "reset" to 0 when they become relevant again. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (NeverAsPin, EditCondition = "!bUpdateInactiveAnimation"))
	bool bResetOnChange;
	
	/* Animation entry. */
	UPROPERTY(EditAnywhere, EditFixedSize, Category = "Input")
	TArray<FPaperZDAnimDataLink> Animation;

	/* Old relevancy value. */
	int32 OldSelectValue;

public:
	//ctor
	FPaperZDAnimNode_SelectByInt();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface

protected:
	/* Obtain the current selected value, clamped. */
	int32 GetSelectedValue() const;
};
