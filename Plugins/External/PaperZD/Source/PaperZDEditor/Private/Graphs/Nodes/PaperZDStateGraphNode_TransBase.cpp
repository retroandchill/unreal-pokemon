// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDStateGraphNode_TransBase.h"
#include "Graphs/PaperZDAnimTransitionGraph.h"
#include "Graphs/PaperZDAnimTransitionGraphSchema.h"
#include "Editors/Util/PaperZDEditorUtils.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "EdGraphUtilities.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDStateGraphNode_TransBase)
#endif

//////////////////////////////////////////////////////////////////////////
//// PaperZD Anim Graph Node
//////////////////////////////////////////////////////////////////////////

UPaperZDStateGraphNode_TransBase::UPaperZDStateGraphNode_TransBase(const FObjectInitializer& ObjectInitializer)
	: Super()
{
}

void UPaperZDStateGraphNode_TransBase::CreateBoundGraph()
{
	// Create a new animation graph
	check(BoundGraph == NULL);
	BoundGraph = FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UPaperZDAnimTransitionGraph::StaticClass(), UPaperZDAnimTransitionGraphSchema::StaticClass());
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

void UPaperZDStateGraphNode_TransBase::PostPlacedNewNode()
{
	CreateBoundGraph();
}

void UPaperZDStateGraphNode_TransBase::DestroyNode()
{
	// BoundGraph may be shared with another graph, if so, don't remove it here
	UEdGraph* GraphToRemove = GetBoundGraph();
	ClearBoundGraph();
	
	Super::DestroyNode();

	if (GraphToRemove)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FPaperZDEditorUtils::RemoveGraph(Blueprint, GraphToRemove);
	}
}

void UPaperZDStateGraphNode_TransBase::ClearBoundGraph()
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

FString UPaperZDStateGraphNode_TransBase::GetNodeName() const
{
	return BoundGraph ? *(BoundGraph->GetName()) : TEXT("(null)");
}
