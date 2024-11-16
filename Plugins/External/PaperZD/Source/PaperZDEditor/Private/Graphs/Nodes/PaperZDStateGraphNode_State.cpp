// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDStateGraphNode_State.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Graphs/Nodes/Slate/SPaperZDStateGraphNode_State.h"
#include "Graphs/PaperZDAnimGraph.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Editors/Util/PaperZDEditorUtils.h"
#include "EdGraph/EdGraph.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "EdGraphUtilities.h"
#include "Kismet2/KismetEditorUtilities.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDStateGraphNode_State)
#endif

UPaperZDStateGraphNode_State::UPaperZDStateGraphNode_State(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	Name_DEPRECATED = FText::FromString(TEXT("State"));
	bCanRenameNode = true;
	bShouldLoop_DEPRECATED = true;
}

FText UPaperZDStateGraphNode_State::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(GetNodeName());;
}

void UPaperZDStateGraphNode_State::AllocateDefaultPins()
{
	FCreatePinParams PinParams;
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT(""), NULL, TEXT("In"), PinParams);
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT(""), NULL, TEXT("Out"), PinParams);
}

void UPaperZDStateGraphNode_State::PostPlacedNewNode()
{
	// Create a new animation graph
	check(BoundGraph == NULL);
	BoundGraph = FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UPaperZDAnimGraph::StaticClass(), UPaperZDAnimGraphSchema::StaticClass());
	check(BoundGraph);
	BoundGraph->bAllowDeletion = false;

	// Find an interesting name
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, GetDesiredNewNodeName());

	// Initialize the anim graph
	const UEdGraphSchema* Schema = BoundGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*BoundGraph);

	// Add the new graph as a child of our parent graph
	UEdGraph* ParentGraph = GetGraph();
	if (ParentGraph->SubGraphs.Find(BoundGraph) == INDEX_NONE)
	{
		ParentGraph->SubGraphs.Add(BoundGraph);
	}
}

void UPaperZDStateGraphNode_State::DestroyNode()
{
	UEdGraph* GraphToRemove = BoundGraph;
	BoundGraph = nullptr;
	ClearBoundGraph();

	Super::DestroyNode();

	if (GraphToRemove)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FPaperZDEditorUtils::RemoveGraph(Blueprint, GraphToRemove);
	}
}

void UPaperZDStateGraphNode_State::PostPasteNode()
{
	for (UEdGraphNode* GraphNode : BoundGraph->Nodes)
	{
		GraphNode->CreateNewGuid();
		GraphNode->PostPasteNode();
		GraphNode->ReconstructNode();
	}

	Super::PostPasteNode();
}

void UPaperZDStateGraphNode_State::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != NULL)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

TSharedPtr<SGraphNode> UPaperZDStateGraphNode_State::CreateVisualWidget()
{
	return SNew(SPaperZDStateGraphNode_State, this);
}

FString UPaperZDStateGraphNode_State::GetNodeName() const
{
	return BoundGraph != nullptr ? *(BoundGraph->GetName()) : TEXT("(null)");
}

FString UPaperZDStateGraphNode_State::GetDesiredNewNodeName() const
{
	return TEXT("State");
}

void UPaperZDStateGraphNode_State::GetTransitions(TArray<UPaperZDStateGraphNode_Transition *>& OutTransitions) const
{
	for (auto Pin : GetOutputPin()->LinkedTo)
	{
		UPaperZDStateGraphNode_Transition* TransitionNode = Cast<UPaperZDStateGraphNode_Transition>(Pin->GetOwningNode());
		if (TransitionNode)
		{
			OutTransitions.Add(TransitionNode);
		}			
	}
}

void UPaperZDStateGraphNode_State::ClearBoundGraph()
{
	// Remove the graph from its parent
	UEdGraph* ParentGraph = GetGraph();

	int32 index = ParentGraph->SubGraphs.Find(BoundGraph);
	if (index != INDEX_NONE)
	{
		ParentGraph->SubGraphs.RemoveAt(index);
	}

	BoundGraph = nullptr;
}
