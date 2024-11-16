// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_LayerAnimations.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_LayerAnimations)
#endif

FPaperZDAnimNode_LayerAnimations::FPaperZDAnimNode_LayerAnimations()
{
	AnimationLayer.AddDefaulted(2);
	LayerWeight = { 1.0f, 1.0f };
}

void FPaperZDAnimNode_LayerAnimations::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	for (FPaperZDAnimDataLink& Anim : AnimationLayer)
	{
		Anim.Initialize(InitContext);
	}
}

void FPaperZDAnimNode_LayerAnimations::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	//Forward the update to every layer
	for (FPaperZDAnimDataLink& Anim : AnimationLayer)
	{
		Anim.Update(UpdateContext);
	}
}

void FPaperZDAnimNode_LayerAnimations::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	//Obtain the data from each layer
	for (int32 i = 0; i < AnimationLayer.Num(); i++)
	{
		FPaperZDAnimDataLink& Anim = AnimationLayer[i];
		FPaperZDAnimationPlaybackData LayerData;
		Anim.Evaluate(LayerData);

		//If this is the base layer, copy the data from there
		if (i == 0)
		{
			OutData.DirectionalAngle = LayerData.DirectionalAngle;
		}

		//Add the layer data
		OutData.WeightedAnimations.Reserve(LayerData.WeightedAnimations.Num());
		for (const FPaperZDWeightedAnimation& WeightedAnim : LayerData.WeightedAnimations)
		{
			FPaperZDWeightedAnimation LayeredAnim = WeightedAnim;
			LayeredAnim.Layer = i;
			LayeredAnim.LayerWeight = LayerWeight[i];

			OutData.WeightedAnimations.Add(LayeredAnim);
		}
	}
}
