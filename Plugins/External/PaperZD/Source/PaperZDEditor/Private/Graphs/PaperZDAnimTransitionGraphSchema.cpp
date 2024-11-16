// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/PaperZDAnimTransitionGraphSchema.h"
#include "Graphs/PaperZDAnimTransitionGraph.h"
#include "Graphs/Nodes/PaperZDTransitionGraphNode_Result.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Conduit.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimTransitionGraphSchema)
#endif

#define LOCTEXT_NAMESPACE "PaperZDAnimGraphSchema"

UPaperZDAnimTransitionGraphSchema::UPaperZDAnimTransitionGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super()
{
}

void UPaperZDAnimTransitionGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UPaperZDTransitionGraphNode_Result> NodeCreator(Graph);
	UPaperZDTransitionGraphNode_Result* ResultSinkNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(ResultSinkNode, FNodeMetadata::DefaultGraphNode);
}

bool UPaperZDAnimTransitionGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	return false;
}

void UPaperZDAnimTransitionGraphSchema::GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const
{
	DisplayInfo.PlainName = FText::FromString(Graph.GetName());

	const UPaperZDStateGraphNode_TransBase* TransNode = Cast<const UPaperZDStateGraphNode_TransBase>(Graph.GetOuter());
	if (TransNode)
	{
		if (TransNode->IsA(UPaperZDStateGraphNode_Transition::StaticClass()))
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), TransNode->GetNodeTitle(ENodeTitleType::FullTitle));
			DisplayInfo.PlainName = FText::Format(NSLOCTEXT("PaperZD", "TransitionRuleGraphTitle", "{NodeTitle} (rule)"), Args);
		}
		else if (TransNode->IsA(UPaperZDStateGraphNode_Conduit::StaticClass()))
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), TransNode->GetNodeTitle(ENodeTitleType::FullTitle));
			DisplayInfo.PlainName = FText::Format(NSLOCTEXT("PaperZD", "ConduitRuleGraphTitle", "{NodeTitle} (conduit rule)"), Args);
		}
	}

	DisplayInfo.DisplayName = DisplayInfo.PlainName;
}

#undef LOCTEXT_NAMESPACE