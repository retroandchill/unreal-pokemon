// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Compilers/Handles/PaperZDAnimBPCompilerHandle_CacheAnimation.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_CacheAnimation.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_UseCachedAnimation.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Sink.h"
#include "Graphs/PaperZDStateMachineGraph.h"

void FPaperZDAnimBPCompilerHandle_CacheAnimation::Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess)
{
	InCompilerAccess.OnPreProcessAnimationNodes().AddRaw(this, &FPaperZDAnimBPCompilerHandle_CacheAnimation::PreProcessAnimationNodes);
	InCompilerAccess.OnPostProcessAnimationNodes().AddRaw(this, &FPaperZDAnimBPCompilerHandle_CacheAnimation::PostProcessAnimationNodes);
}

void FPaperZDAnimBPCompilerHandle_CacheAnimation::PreProcessAnimationNodes(TArrayView<UPaperZDAnimGraphNode_Base*> InAnimNodes, FPaperZDAnimBPCompilerAccess& InCompilerAccess, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	//Gather every cache animation node for later referral with their "UseCachedAnimation" node counterpart.
	for (UPaperZDAnimGraphNode_Base* Node : InAnimNodes)
	{
		if (UPaperZDAnimGraphNode_CacheAnimation* CacheAnimationNode = Cast<UPaperZDAnimGraphNode_CacheAnimation>(Node))
		{
			CacheAnimationNodes.Add(CacheAnimationNode->GetCacheName(), CacheAnimationNode);
		}
	}
}

void FPaperZDAnimBPCompilerHandle_CacheAnimation::PostProcessAnimationNodes(TArrayView<UPaperZDAnimGraphNode_Base*> InAnimNodes, FPaperZDAnimBPCompilerAccess& InCompilerAccess, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	//Do a sanity check against the Cache and Use nodes, to make sure there are no circular references inside.
	CheckForCircularReferences(InCompilerAccess);
}

void FPaperZDAnimBPCompilerHandle_CacheAnimation::CheckForCircularReferences(FPaperZDAnimBPCompilerAccess& InCompilerAccess)
{
	//First we need to filter the AnimNodes, as we only want to check for the true sink node.
	TArray<UPaperZDAnimGraphNode_Sink*> RootNodes;
	InCompilerAccess.GetConsolidatedEventGraph()->GetNodesOfClass<UPaperZDAnimGraphNode_Sink>(RootNodes);

	//Do a recursive search to find any circular references for cached animations
	for (UPaperZDAnimGraphNode_Sink* Node : RootNodes)
	{
		TSet<UPaperZDAnimGraphNode_CacheAnimation*> VisitedNodes;
		CircularReferenceCheck_TraverseNode(Node, InCompilerAccess, VisitedNodes);
	}
}

void FPaperZDAnimBPCompilerHandle_CacheAnimation::CircularReferenceCheck_TraverseNode(UPaperZDAnimGraphNode_Base* RootNode, FPaperZDAnimBPCompilerAccess& InCompilerAccess, TSet<UPaperZDAnimGraphNode_CacheAnimation*>& VisitedNodes)
{
	//Obtain all the nodes that are linked to this one
	TArray<UPaperZDAnimGraphNode_Base*> Nodes;
	InCompilerAccess.GetLinkedAnimNodes(RootNode, Nodes);

	//Each branch should be independent of each other as to avoid the system thinking that a recursion happened when in reality it just found two nodes in another branch
	//for this we store the original state of the visited nodes as a reference and make every branch create a copy of it, while we collect the visited nodes on the original set
	TSet<UPaperZDAnimGraphNode_CacheAnimation*> InternalReferenceSet = VisitedNodes;

	//Check for nodes that can eventually end up referencing a CacheAnimation node
	for (UPaperZDAnimGraphNode_Base* AnimNode : Nodes)
	{
		if (UPaperZDAnimGraphNode_UseCachedAnimation* UseCacheNode = Cast<UPaperZDAnimGraphNode_UseCachedAnimation>(AnimNode))
		{
			UPaperZDAnimGraphNode_CacheAnimation* CacheAnimation = UseCacheNode->LinkedCacheNode.Get();
			if (InternalReferenceSet.Contains(CacheAnimation))
			{
				InCompilerAccess.GetMessageLog().Error(TEXT("Infinite recursion detected between nodes '@@' and '@@'"), CacheAnimation, UseCacheNode);
				return;
			}
			else
			{
				//Create a branch independent copy
				TSet<UPaperZDAnimGraphNode_CacheAnimation*> BranchSet = InternalReferenceSet;
				BranchSet.Add(CacheAnimation);
				CircularReferenceCheck_TraverseNode(CacheAnimation, InCompilerAccess, BranchSet);
				
				//Now that the recursion has ended, add the accumulated nodes
				VisitedNodes.Union(BranchSet);
			}
		}
		else if (UPaperZDAnimGraphNode_StateMachine* MachineNode = Cast<UPaperZDAnimGraphNode_StateMachine>(AnimNode))
		{
			//Check both the state graphs and the transitionals
			for (UEdGraph* StateGraph : MachineNode->GetStateMachineGraph()->SubGraphs)
			{
				TArray<UPaperZDAnimGraphNode_Sink*> ResultNodes;
				StateGraph->GetNodesOfClass(ResultNodes);

				// We should only get one here but doesn't hurt to loop here in case that changes
				for (UPaperZDAnimGraphNode_Sink* ResultNode : ResultNodes)
				{
					//Create a branch independent copy
					TSet<UPaperZDAnimGraphNode_CacheAnimation*> BranchSet = InternalReferenceSet;
					CircularReferenceCheck_TraverseNode(ResultNode, InCompilerAccess, BranchSet);

					//Now that the recursion has ended, add the accumulated nodes
					VisitedNodes.Union(BranchSet);
				}
			}
		}
	}
}
