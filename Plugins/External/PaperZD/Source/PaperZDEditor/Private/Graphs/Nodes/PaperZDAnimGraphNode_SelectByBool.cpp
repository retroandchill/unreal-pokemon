// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_SelectByBool.h"
#include "Kismet2/CompilerResultsLog.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_SelectByBool)
#endif

UPaperZDAnimGraphNode_SelectByBool::UPaperZDAnimGraphNode_SelectByBool()
	: Super()
{}

FText UPaperZDAnimGraphNode_SelectByBool::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Select Animation by Bool"));
}

FLinearColor UPaperZDAnimGraphNode_SelectByBool::GetNodeTitleColor() const
{
	return FLinearColor(0.2f, 0.8f, 0.2f);
}


FString UPaperZDAnimGraphNode_SelectByBool::GetNodeCategory() const
{
	return TEXT("Select/Blends");
}

void UPaperZDAnimGraphNode_SelectByBool::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
{
	//We should have both animation links connected
	UEdGraphPin* TrueAnimationPin = FindPin(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_SelectByBool, TrueAnimation), EGPD_Input);
	UEdGraphPin* FalseAnimationPin = FindPin(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_SelectByBool, FalseAnimation), EGPD_Input);
	check(TrueAnimationPin);
	check(FalseAnimationPin);

	if (TrueAnimationPin->LinkedTo.Num() == 0)
	{
		MessageLog.Error(TEXT("@@ has nothing bound to its 'True' animation pin"), this);
	}

	if (FalseAnimationPin->LinkedTo.Num() == 0)
	{
		MessageLog.Error(TEXT("@@ has nothing bound to its 'False' animation pin"), this);
	}
}

