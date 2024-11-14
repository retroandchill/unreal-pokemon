// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Sink.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "K2Node_Knot.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphSchema)
#endif

#define LOCTEXT_NAMESPACE "PaperZDAnimGraphSchema"

//////////////////////////////////////////////////////////////////////////
/// PaperZD AnimGraph Schema
UPaperZDAnimGraphSchema::UPaperZDAnimGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	DefaultEvaluationHandlerName = TEXT("EvaluateGraphExposedInputs");
}

FLinearColor UPaperZDAnimGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	if (IsAnimationDataPin(PinType))
	{
		return FLinearColor::White;
	}

	return Super::GetPinTypeColor(PinType);
}

EGraphType UPaperZDAnimGraphSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_Animation;
}

void UPaperZDAnimGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UPaperZDAnimGraphNode_Sink> NodeCreator(Graph);
	UPaperZDAnimGraphNode_Sink* ResultSinkNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(ResultSinkNode, FNodeMetadata::DefaultGraphNode);
}

void UPaperZDAnimGraphSchema::GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const
{
	DisplayInfo.PlainName = FText::FromString(Graph.GetName());

	const UPaperZDStateGraphNode_Transition* TransNode = Cast<const UPaperZDStateGraphNode_Transition>(Graph.GetOuter());
	if (TransNode)
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), TransNode->GetNodeTitle(ENodeTitleType::FullTitle));
		DisplayInfo.PlainName = FText::Format(NSLOCTEXT("PaperZD", "TransitionAnimationGraphTitle", "{NodeTitle} (trans. animation)"), Args);
	}

	DisplayInfo.DisplayName = DisplayInfo.PlainName;
}

const FPinConnectionResponse UPaperZDAnimGraphSchema::DetermineConnectionResponseOfCompatibleTypedPins(const UEdGraphPin* PinA, const UEdGraphPin* PinB, const UEdGraphPin* InputPin, const UEdGraphPin* OutputPin) const
{
	if (IsAnimationDataPin(OutputPin->PinType) && IsAnimationDataPin(InputPin->PinType))
	{
		if ((OutputPin->LinkedTo.Num() > 0) || (InputPin->LinkedTo.Num() > 0))
		{
			const ECanCreateConnectionResponse ReplyBreakOutputs = CONNECT_RESPONSE_BREAK_OTHERS_AB;
			return FPinConnectionResponse(ReplyBreakOutputs, TEXT("Replace existing connections"));
		}
	}

	// Fall back to standard K2 rules
	return Super::DetermineConnectionResponseOfCompatibleTypedPins(PinA, PinB, InputPin, OutputPin);
}

bool UPaperZDAnimGraphSchema::ArePinsCompatible(const UEdGraphPin* PinA, const UEdGraphPin* PinB, const UClass* CallingContext /* = NULL */, bool bIgnoreArray /* = false */) const
{
	// Disallow animation pins connecting to wildcards (apart from reroute nodes)
	if (IsAnimationDataPin(PinA->PinType) && PinB->PinType.PinCategory == PC_Wildcard)
	{
		return Cast<UK2Node_Knot>(PinB->GetOwningNode()) != nullptr;
	}
	else if (IsAnimationDataPin(PinB->PinType) && PinA->PinType.PinCategory == PC_Wildcard)
	{
		return Cast<UK2Node_Knot>(PinA->GetOwningNode()) != nullptr;
	}

	//Fallback to parent method
	return Super::ArePinsCompatible(PinA, PinB, CallingContext, bIgnoreArray);
}

bool UPaperZDAnimGraphSchema::DoesSupportAnimNotifyActions() const
{
	return false;
}

bool UPaperZDAnimGraphSchema::IsAnimationDataPin(const FEdGraphPinType& PinType)
{
	UScriptStruct* AnimDataLinkStruct = FPaperZDAnimDataLink::StaticStruct();
	return (PinType.PinCategory == UPaperZDAnimGraphSchema::PC_Struct) && (PinType.PinSubCategoryObject == AnimDataLinkStruct);
}

#undef LOCTEXT_NAMESPACE
