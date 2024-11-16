// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_SetDirectionality.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_SetDirectionality)
#endif

FPaperZDAnimNode_SetDirectionality::FPaperZDAnimNode_SetDirectionality()
	: Input(FVector2D::ZeroVector)
	, CachedDirectionalAngle(0.0f)
{}

void FPaperZDAnimNode_SetDirectionality::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	Animation.Initialize(InitContext);
}

void FPaperZDAnimNode_SetDirectionality::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	//Cache the directional angle
	static const FVector2D TopPosition(0.0f, 1.0f);
	const float Sign = Input.X != 0.0f ? FMath::Sign(Input.X) : 1.0f;
	const float AngleRad = FMath::Acos(Input.GetSafeNormal() | TopPosition) * Sign;
	CachedDirectionalAngle = FMath::RadiansToDegrees(AngleRad);

	//Update the relevant animation
	Animation.Update(UpdateContext);
}

void FPaperZDAnimNode_SetDirectionality::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	//Ask the animation to evaluate itself, and overwrite the directional angle
	Animation.Evaluate(OutData);
	OutData.DirectionalAngle = CachedDirectionalAngle;
}
