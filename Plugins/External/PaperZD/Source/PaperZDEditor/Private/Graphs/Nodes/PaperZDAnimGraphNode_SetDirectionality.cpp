// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_SetDirectionality.h"
#include "Kismet2/CompilerResultsLog.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_SetDirectionality)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_SetDirectionality::UPaperZDAnimGraphNode_SetDirectionality()
	: Super()
{}

FString UPaperZDAnimGraphNode_SetDirectionality::GetNodeCategory() const
{
	return TEXT("Animation");
}

void UPaperZDAnimGraphNode_SetDirectionality::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
{
	//We should have both animation links connected
	UEdGraphPin* AnimationPin = FindPin(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_SetDirectionality, Animation), EGPD_Input);
	check(AnimationPin);

	if (AnimationPin->LinkedTo.Num() == 0)
	{
		MessageLog.Error(TEXT("@@ has nothing bound to its animation pin"), this);
	}
}

FLinearColor UPaperZDAnimGraphNode_SetDirectionality::GetNodeTitleColor() const
{
	return FLinearColor::White;
}

FText UPaperZDAnimGraphNode_SetDirectionality::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("SetDirectionalityNodeTitle", "Set Directionality");
}

#undef LOCTEXT_NAMESPACE
