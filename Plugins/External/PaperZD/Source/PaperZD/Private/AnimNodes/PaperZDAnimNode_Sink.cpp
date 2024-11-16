// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_Sink.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_Sink)
#endif

FPaperZDAnimNode_Sink::FPaperZDAnimNode_Sink()
	: Name(NAME_None)
{}

bool FPaperZDAnimNode_Sink::IsMainSinkNode() const
{
	//@NOTE potentially use a better method for detecting the main graph
	static const FName MainGraphName(TEXT("AnimGraph"));
	return Name.IsEqual(MainGraphName);
}

bool FPaperZDAnimNode_Sink::HasAnimationData() const
{
	return Result.LinkID != INDEX_NONE;
}

void FPaperZDAnimNode_Sink::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	Result.Initialize(InitContext);
}

void FPaperZDAnimNode_Sink::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	Result.Update(UpdateContext);
}

void FPaperZDAnimNode_Sink::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	Result.Evaluate(OutData);
}
