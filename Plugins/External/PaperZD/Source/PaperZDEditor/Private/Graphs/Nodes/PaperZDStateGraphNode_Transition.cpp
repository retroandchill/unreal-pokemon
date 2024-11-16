// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Graphs/Nodes/Slate/SPaperZDStateGraphNode_Transition.h"
#include "Graphs/PaperZDAnimGraph.h"
#include "Graphs/PaperZDAnimGraphSchema_Transitional.h"
#include "Editors/Util/PaperZDEditorUtils.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/KismetEditorUtilities.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDStateGraphNode_Transition)
#endif

#define LOCTEXT_NAMESPACE "ZD_Nodes"

UPaperZDStateGraphNode_Transition::UPaperZDStateGraphNode_Transition(const FObjectInitializer& ObjectInitializer)
	: Super()
	, Priority(0)
	, Color(FColor::White)
	, bPlayTransitionalAnimations(false)
	, AnimTransitionalGraph(nullptr)
{}

void UPaperZDStateGraphNode_Transition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPaperZDStateGraphNode_Transition, bPlayTransitionalAnimations))
	{
		UpdateAnimTransitionalGraphState();
	}
}

void UPaperZDStateGraphNode_Transition::CreateConnections(UPaperZDStateGraphNode* PreviousState, UPaperZDStateGraphNode* NextState)
{
	// Previous to this
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();
	
	PreviousState->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(PreviousState->GetOutputPin());

	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	NextState->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(NextState->GetInputPin());
}

void UPaperZDStateGraphNode_Transition::AllocateDefaultPins()
{
	FCreatePinParams PinParams;
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT(""), NULL, TEXT("In"), PinParams);
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT(""), NULL, TEXT("Out"), PinParams);
	Outputs->bHidden = true;
}

void UPaperZDStateGraphNode_Transition::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		// Commit suicide, transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}
UPaperZDStateGraphNode* UPaperZDStateGraphNode_Transition::GetFromNode() const
{
	if (GetInputPin()->LinkedTo.Num() != 0)
	{
		return Cast<UPaperZDStateGraphNode>(GetInputPin()->LinkedTo[0]->GetOwningNode());
	}

	return nullptr;
}

UPaperZDStateGraphNode* UPaperZDStateGraphNode_Transition::GetToNode() const
{
	if (GetOutputPin()->LinkedTo.Num() != 0)
	{
		return Cast<UPaperZDStateGraphNode>(GetOutputPin()->LinkedTo[0]->GetOwningNode());
	}

	return nullptr;
}

void UPaperZDStateGraphNode_Transition::OpenAnimTransitionalGraph() const
{
	if (AnimTransitionalGraph)
	{
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(AnimTransitionalGraph);
	}
}

TSharedPtr<SGraphNode> UPaperZDStateGraphNode_Transition::CreateVisualWidget()
{
	return SNew(SPaperZDStateGraphNode_Transition, this);
}

void UPaperZDStateGraphNode_Transition::PostPasteNode()
{
// 	if (GetBoundGraph() == NULL)
// 	{
// 		// fail-safe, create empty transition graph
// 		CreateBoundGraph();
// 	}

	for (UEdGraphNode* GraphNode : GetBoundGraph()->Nodes)
	{
		GraphNode->CreateNewGuid();
		GraphNode->PostPasteNode();
		GraphNode->ReconstructNode();
	}

	if (AnimTransitionalGraph)
	{
		for (UEdGraphNode* GraphNode : AnimTransitionalGraph->Nodes)
		{
			GraphNode->CreateNewGuid();
			GraphNode->PostPasteNode();
			GraphNode->ReconstructNode();
		}

		//Update the menu entry if needed
		UpdateAnimTransitionalGraphState();
	}

	Super::PostPasteNode();

	// Remove nodes that aren't fully linked
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->LinkedTo.Num() == 0)
		{
			DestroyNode();
			break;
		}
	}
}

void UPaperZDStateGraphNode_Transition::DestroyNode()
{
	Super::DestroyNode();

	//Remove the transitional AnimGraph
	UEdGraph* GraphToRemove = AnimTransitionalGraph;
	AnimTransitionalGraph = nullptr;

	if (GraphToRemove)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FPaperZDEditorUtils::RemoveGraph(Blueprint, GraphToRemove);
	}
}

FString UPaperZDStateGraphNode_Transition::GetDesiredNewNodeName() const
{
	return TEXT("Transition");
}

FText UPaperZDStateGraphNode_Transition::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UPaperZDStateGraphNode* PrevNode = GetFromNode();
	UPaperZDStateGraphNode* ToNode = GetToNode();

	//Should always have a prev/to node pair
	if (PrevNode && ToNode)
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("PrevState"), FText::FromString(PrevNode->GetNodeName()));
		Args.Add(TEXT("NextState"), FText::FromString(ToNode->GetNodeName()));
		return FText::Format(LOCTEXT("PrevStateToNewState", "{PrevState} to {NextState}"), Args);
	}
	else
	{
		return FText::FromString(TEXT("~Invalid Transition~"));
	}
}

void UPaperZDStateGraphNode_Transition::CreateAnimTransitionalGraph()
{
	// Create a new animation graph
	check(!AnimTransitionalGraph)
	AnimTransitionalGraph = FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UPaperZDAnimGraph::StaticClass(), UPaperZDAnimGraphSchema_Transitional::StaticClass());
	check(AnimTransitionalGraph);
	AnimTransitionalGraph->bAllowDeletion = false;

	// Find an interesting name
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(AnimTransitionalGraph, NameValidator, TEXT("AnimTransitionGraph"));

	// Initialize the anim graph
	const UEdGraphSchema* Schema = AnimTransitionalGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*AnimTransitionalGraph);

	// Conditionally add/remove the transitional graph from its parent graph
	UpdateAnimTransitionalGraphState();
}

void UPaperZDStateGraphNode_Transition::UpdateAnimTransitionalGraphState()
{
	//Make sure we have a transitional graph available
	if (!AnimTransitionalGraph)
	{
		CreateAnimTransitionalGraph();
	}

	check(AnimTransitionalGraph);
	if (bPlayTransitionalAnimations)
	{
		// Add the new graph as a child of our parent graph
		UEdGraph* ParentGraph = GetGraph();
		if (ParentGraph->SubGraphs.Find(AnimTransitionalGraph) == INDEX_NONE)
		{
			ParentGraph->SubGraphs.Add(AnimTransitionalGraph);
		}
	}
	else
	{
		// Remove graph
		UEdGraph* ParentGraph = GetGraph();
		if (ParentGraph->SubGraphs.Find(AnimTransitionalGraph) != INDEX_NONE)
		{
			ParentGraph->SubGraphs.Remove(AnimTransitionalGraph);
		}
	}
}

#undef LOCTEXT_NAMESPACE
