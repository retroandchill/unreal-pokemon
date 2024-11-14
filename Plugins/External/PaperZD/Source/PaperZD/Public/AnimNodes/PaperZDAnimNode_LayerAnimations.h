// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_LayerAnimations.generated.h"

/**
 * Sets the animations in different weighted layers, running parallel and independent to each other.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_LayerAnimations : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Animation entry. */
	UPROPERTY(EditAnywhere, EditFixedSize, Category = "Input")
	TArray<FPaperZDAnimDataLink> AnimationLayer;

	/* Weights for each layer. */
	UPROPERTY(EditAnywhere, EditFixedSize, Category = "Input", meta = (ShowPinByDefault, UIMin = "0.0", ClampMin = "0.0", UIMax = "1.0", ClampMax = "1.0"))
	TArray<float> LayerWeight;

public:
	//ctor
	FPaperZDAnimNode_LayerAnimations();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface
};
