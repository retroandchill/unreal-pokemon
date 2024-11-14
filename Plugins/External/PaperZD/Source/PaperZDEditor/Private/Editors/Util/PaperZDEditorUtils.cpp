// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Util/PaperZDEditorUtils.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Graphs/PaperZDStateMachineGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"

void FPaperZDEditorUtils::RemoveGraph(UBlueprint* Blueprint, class UEdGraph* GraphToRemove)
{
	TArray<UEdGraph*> Graphs;
	Blueprint->GetAllGraphs(Graphs);


	//Normally we would use FBlueprintEditorUtils::RemoveGraph, but this method doesn't handle
	//the recursion that the AnimStateMachine has (and only handles UE4 AnimStateMachine's by
	//having the behavior hardcoded).
	//We will iterate over the OUTER chain and remove ourselves from our parent graph.
	for (UObject* TestOuter = GraphToRemove->GetOuter(); TestOuter; TestOuter = TestOuter->GetOuter())
	{
		if (TestOuter == Blueprint)
		{
			//There's a chance the graph is opened as a document tab
			// Can't just call Remove, the object is wrapped in a struct
			for (int EditedDocIdx = 0; EditedDocIdx < Blueprint->LastEditedDocuments.Num(); ++EditedDocIdx)
			{
				if (Blueprint->LastEditedDocuments[EditedDocIdx].EditedObjectPath.ResolveObject() == GraphToRemove)
				{
					Blueprint->LastEditedDocuments.RemoveAt(EditedDocIdx);
					break;
				}
			}
		}
		else if (UEdGraph* OuterGraph = Cast<UEdGraph>(TestOuter))
		{
			// remove ourselves
			OuterGraph->Modify();
			OuterGraph->SubGraphs.Remove(GraphToRemove);
		}
		else if (!(Cast<UPaperZDStateGraphNode>(TestOuter) ||
			Cast<UPaperZDAnimGraphNode_StateMachine>(TestOuter)))
		{
			break;
		}
	}

	//Handle deletion of any StateMachine
	TArray<UPaperZDAnimGraphNode_StateMachine*> StateMachineNodes;
	GraphToRemove->GetNodesOfClassEx<UPaperZDAnimGraphNode_StateMachine>(StateMachineNodes);
	for (UPaperZDAnimGraphNode_StateMachine* SMNode : StateMachineNodes)
	{
		RemoveGraph(Blueprint, SMNode->GetStateMachineGraph());
	}

	//Handle deletion of any internal StateMachine node
	TArray<UPaperZDStateGraphNode*> AnimStateNodes;
	GraphToRemove->GetNodesOfClassEx<UPaperZDStateGraphNode>(AnimStateNodes);
	for (UPaperZDStateGraphNode* Node : AnimStateNodes)
	{
		UEdGraph* BoundGraph = Node->GetBoundGraph();
		if (BoundGraph)
		{
			RemoveGraph(Blueprint, BoundGraph);
		}

		//Special case for AnimTransitionalGraphs
		UPaperZDStateGraphNode_Transition* PossibleTransition = Cast<UPaperZDStateGraphNode_Transition>(Node);
		if (PossibleTransition && PossibleTransition->GetAnimTransitionalGraph() != nullptr)
		{
			RemoveGraph(Blueprint, PossibleTransition->GetAnimTransitionalGraph());
		}
	}

	//Delete this graph by using the standard BlueprintEditor calls
	FBlueprintEditorUtils::RemoveGraph(Blueprint, GraphToRemove, EGraphRemoveFlags::Recompile);
	Graphs.Empty();
	Blueprint->GetAllGraphs(Graphs);
}
