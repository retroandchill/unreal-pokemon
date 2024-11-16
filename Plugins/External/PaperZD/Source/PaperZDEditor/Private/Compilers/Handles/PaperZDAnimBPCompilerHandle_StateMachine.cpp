// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Compilers/Handles/PaperZDAnimBPCompilerHandle_StateMachine.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "Graphs/Nodes/PaperZDTransitionGraphNode_Result.h"
#include "Graphs/Nodes/PaperZDK2Node_AnimGetter.h"
#include "Graphs/PaperZDAnimTransitionGraph.h"
#include "AnimNodes/PaperZDAnimStateMachine.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraphUtilities.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_FunctionResult.h"

void FPaperZDAnimBPCompilerHandle_StateMachine::Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess)
{
	InCompilerAccess.OnStartCompilingClass().AddRaw(this, &FPaperZDAnimBPCompilerHandle_StateMachine::HandleStartCompilingClass);
	InCompilerAccess.OnPostProcessAnimationNodes().AddRaw(this, &FPaperZDAnimBPCompilerHandle_StateMachine::PostProcessAnimationNodes);
}

int32 FPaperZDAnimBPCompilerHandle_StateMachine::ProcessAnimationGraph(UEdGraph* SourceGraph, UPaperZDAnimGraphNode_Base* SourceRootNode, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	//Because the inner AnimationGraphs (inside the state nodes and transitional nodes) are not processed in the "MergeUbergraphPagesIn" method of the compiler
	//due to them being inside the StateMachine nodes, this method will re-create the steps needed for the nodes to be added to the consolidated event graph
	//and be processed as they should.

	//The first step is to clone the graph, just as the "MoveGraph" lambda.
	//We need a reference to the clones graph, as we need to only process the animation nodes of this graph and not on the ConsolidatedEventGraph
	//which would mean that we process the node that called this handle again. 
	//We are using the consolidated event graph because the "ValidateGraphIsWellFormed" checks for an outer on the node, and we trigger that method before moving the nodes.
	UEdGraph* ClonedGraph = FEdGraphUtilities::CloneGraph(SourceGraph, InCompilationContext.GetConsolidatedEventGraph(), &InCompilationContext.GetMessageLog(), true);

	//This is the equivalent of the "ProcessAllAnimationNodes" method
	UPaperZDAnimGraphNode_Base* TargetRootNode = nullptr;
	{
		//Obtain the root node
		TArray<UPaperZDAnimGraphNode_Base*> RootAnimNodeList;
		ClonedGraph->GetNodesOfClass(RootAnimNodeList);

		//Search for the equivalent of the source root node that we cloned
		for (UPaperZDAnimGraphNode_Base* Node : RootAnimNodeList)
		{
			if (InCompilationContext.GetMessageLog().FindSourceObject(Node) == InCompilationContext.GetMessageLog().FindSourceObject(SourceRootNode))
			{
				TargetRootNode = Node;
				break;
			}
		}

		//We should have found a root node
		check(TargetRootNode);

		//Add only the root node we need to process
		TArray<UPaperZDAnimGraphNode_Base*> RootSet;
		RootSet.Add(TargetRootNode);

		InCompilationContext.PruneIsolatedAnimationNodes(RootSet, RootAnimNodeList);
		InCompilationContext.ValidateGraphIsWellFormed(ClonedGraph);
		InCompilationContext.ProcessAnimationNodes(RootAnimNodeList);
	}

	//Finally we move the nodes to the consolidated event graph
	const bool bIsLoading = InCompilationContext.GetAnimBP()->bIsRegeneratingOnLoad || IsAsyncLoading();
	const bool bIsCompiling = InCompilationContext.GetAnimBP()->bBeingCompiled;
	ClonedGraph->MoveNodesToAnotherGraph(InCompilationContext.GetConsolidatedEventGraph(), bIsLoading, bIsCompiling);
	
	//Finally, return the index of the root node
	return InCompilationContext.GetAllocationIndexOfNode(TargetRootNode);
}

void FPaperZDAnimBPCompilerHandle_StateMachine::ProcessTransitionGraph(UEdGraph* SourceGraph, FPaperZDAnimStateMachineTransitionRule& OutTransitionRule, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	UPaperZDTransitionGraphNode_Result* ResultNode = CastChecked<UPaperZDAnimTransitionGraph>(SourceGraph)->GetResultNode();
	check(ResultNode && ResultNode->Pins.Num());
	UEdGraphPin* ResultPin = ResultNode->Pins[0];

	//Depending if the result node is linked to anything, we will create a function graph for it, or just "fast-path" the value to the transition rule
	if (ResultPin->LinkedTo.Num() > 0)
	{
		//Result pin has logic inside, we will create a function graph to hold the logic in place
		UEdGraph* ClonedGraph = FEdGraphUtilities::CloneGraph(SourceGraph, InCompilationContext.GetConsolidatedEventGraph(), &InCompilationContext.GetMessageLog(), true);
		ResultNode = CastChecked<UPaperZDAnimTransitionGraph>(ClonedGraph)->GetResultNode();
		ResultPin = ResultNode->Pins[0];

		//Copy the AnimGetter nodes found on this graph, will be processed after the state machine is complete
		ClonedGraph->GetNodesOfClass(AnimGetterNodes);

		//Create the entry point
		UK2Node_FunctionEntry* FunctionEntry = InCompilationContext.SpawnIntermediateNode<UK2Node_FunctionEntry>(ResultNode, ClonedGraph);
		FunctionEntry->CustomGeneratedFunctionName = GenerateValidTransitionFunctionName(InCompilationContext, TEXT("ZDRule_") + SourceGraph->GetName());
		FunctionEntry->AllocateDefaultPins();

		//Create function output
		UK2Node_FunctionResult* FunctionResult = InCompilationContext.SpawnIntermediateNode<UK2Node_FunctionResult>(ResultNode, ClonedGraph);
		FunctionResult->AllocateDefaultPins();

		//Bind transition rule pins
		UEdGraphPin* FunctionValuePin = FunctionResult->CreatePin(EGPD_Input, ResultPin->PinType, ResultPin->GetFName());
		FunctionValuePin->DefaultValue = ResultPin->DefaultValue; //Stored in case the links get to no logic whatsoever (i.e. chain of reroutes)

		UEdGraphPin* RuleValuePin = ResultPin->LinkedTo[0];
		RuleValuePin->MakeLinkTo(FunctionValuePin);

		//Bind exec nodes
		UEdGraphPin* FunctionEntryExecPin = CastChecked<UEdGraphSchema_K2>(ClonedGraph->GetSchema())->FindExecutionPin(*FunctionEntry, EGPD_Output);
		FunctionResult->GetExecPin()->MakeLinkTo(FunctionEntryExecPin);

		//Remove the original result node and validate
		ResultNode->DestroyNode();
		InCompilationContext.ValidateGraphIsWellFormed(ClonedGraph);
		InCompilationContext.GetGeneratedFunctionGraphs().Add(ClonedGraph);

		//Finally fill the data
		OutTransitionRule.bDynamicRule = true;
		OutTransitionRule.RuleFunctionName = FunctionEntry->CustomGeneratedFunctionName;
	}
	else
	{
		//Pin has no logic connected to it, doesn't make sense to create a whole function for a constant value that will not change.
		//Fast path the value and copy it directly to the transition rule
		OutTransitionRule.bDynamicRule = false;
		OutTransitionRule.bConstantValue = ResultPin->GetDefaultAsString().ToBool();
	}
}

void FPaperZDAnimBPCompilerHandle_StateMachine::HandleStartCompilingClass(const UClass* InClass, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	AnimGetterNodes.Empty();
}

void FPaperZDAnimBPCompilerHandle_StateMachine::PostProcessAnimationNodes(TArrayView<UPaperZDAnimGraphNode_Base*> InAnimNodes, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	//Check every found AnimGetter node and wire them up internally if they are valid
	for (UPaperZDK2Node_AnimGetter* AnimGetter : AnimGetterNodes)
	{
		AutoWireAnimGetter(AnimGetter, InCompilationContext, OutCompiledData);
	}
}

FName FPaperZDAnimBPCompilerHandle_StateMachine::GenerateValidTransitionFunctionName(FPaperZDAnimBPCompilerAccess& InCompilationContext, const FString& InBaseName) const
{
	//First create a set of the generated function graphs that have custom generated function names
	TSet<FName> FunctionNames;
	for (const UEdGraph* Graph : InCompilationContext.GetGeneratedFunctionGraphs())
	{
		//Find the function entry node
		TArray<UK2Node_FunctionEntry*> FunctionEntries;
		Graph->GetNodesOfClass(FunctionEntries);

		//It only makes sense for one node to exist, if any
		if (FunctionEntries.Num() > 0)
		{
			UK2Node_FunctionEntry* EntryNode = FunctionEntries[0];
			if (EntryNode->CustomGeneratedFunctionName != NAME_None)
			{
				FunctionNames.Add(EntryNode->CustomGeneratedFunctionName);
			}
		}
	}

	//Fill with the actual used names of the functions
	FBlueprintEditorUtils::GetFunctionNameList(InCompilationContext.GetAnimBP(), FunctionNames);

	//Need to manually validate the function names, as the GeneratedFunctionNames haven't created a function stub yet, hence they will collide if two of the transitions have the same name
	//This is very probable and non-avoidable when there are nested state machines, as they only generate unique names from their own scope
	FString TestString = InBaseName;
	int32 Count = 0;
	while (FunctionNames.Contains(*TestString))
	{
		TestString = FString::Printf(TEXT("%s_%d"), *InBaseName, Count);
		Count++;
	}
	
	//Return the actual name, now that its available
	return FName(*TestString);
}

void FPaperZDAnimBPCompilerHandle_StateMachine::AutoWireAnimGetter(UPaperZDK2Node_AnimGetter* AnimGetter, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	UEdGraphPin* ReferencedNodeTimePin = nullptr;
	int32 ReferencedNodeIndex = INDEX_NONE;
	int32 SubNodeIndex = INDEX_NONE;
	bool bProcessedNode = false;

	if (AnimGetter->SourceNode)
	{
		UPaperZDAnimGraphNode_Base* ActualSourceNode = InCompilationContext.GetMessageLog().FindSourceObjectTypeChecked<UPaperZDAnimGraphNode_Base>(AnimGetter->SourceNode);
		if (ActualSourceNode)
		{
			UPaperZDAnimGraphNode_Base* ProcessedSourceNode = InCompilationContext.GetSourceNodeToProcessedNodeMap().FindRef(ActualSourceNode);
			if (ProcessedSourceNode)
			{
				bProcessedNode = true;
				ReferencedNodeIndex = InCompilationContext.GetAllocationIndexOfNode(ProcessedSourceNode);
			}
		}
	}

	//@NOTE: Currently we have no easy way of obtaining the indices of the state machine node back from compilation
	//this can be done when we have Debug data (which would effectively map the GraphNode to the BakedNode).
	//Given that we don't presently need state machine AnimGetter calls, we leave this open for later iterations.

	//Check if we actually processed this node
	if (!bProcessedNode)
	{
		InCompilationContext.GetMessageLog().Warning(TEXT("Could not parse @@, was the referenced node deleted?"), AnimGetter);
	}

	check(AnimGetter->IsNodePure());

	//Bind pins
	for (UEdGraphPin* Pin : AnimGetter->Pins)
	{
		// Hook up autowired parameters / pins
		if (Pin->PinName == TEXT("AssetPlayerIndex") || Pin->PinName == TEXT("MachineIndex"))
		{
			Pin->DefaultValue = FString::FromInt(ReferencedNodeIndex);
		}
		else if (Pin->PinName == TEXT("StateIndex") || Pin->PinName == TEXT("TransitionIndex"))
		{
			Pin->DefaultValue = FString::FromInt(SubNodeIndex);
		}
	}
}
