// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Compilers/PaperZDAnimBPCompilerContext.h"

class FCompilerResultsLog;
class FPaperZDAnimDataLinkRecord;
class UPaperZDAnimGraphNode_Base;

/**
 * Compile time accessor to the running "Compiler Context".
 */
class FPaperZDAnimBPCompilerAccess
{
	/* Only grant access during compilation (private constructor, only the compiler can generate this accessor). */
	friend class FPaperZDAnimBPCompilerContext;
	FPaperZDAnimBPCompilerAccess(FPaperZDAnimBPCompilerContext* InCompiler)
		: CompilerContext(InCompiler)
	{
		check(CompilerContext);
	}

	/* Compiler context we're wrapping. */
	FPaperZDAnimBPCompilerContext* CompilerContext;

public:
	/* Request a given compiler handle. */
	template <typename T>
	T* GetHandle()
	{
		TUniquePtr<IPaperZDAnimBPCompilerHandle>* HandlePtr = CompilerContext->CompileHandles.Find(T::GetClassId());
		if (HandlePtr)
		{
			IPaperZDAnimBPCompilerHandle* HandleBase = (*HandlePtr).Get();
			return static_cast<T*>(HandleBase);
		}
		else
		{
			return nullptr;
		}
	}

	/* Spawns an intermediate node associated with the source node (for error purposes) */
	template <typename NodeType>
	NodeType* SpawnIntermediateNode(UEdGraphNode* SourceNode, UEdGraph* ParentGraph = nullptr)
	{
		return CompilerContext->SpawnIntermediateNode<NodeType>(SourceNode, ParentGraph);
	}

	/** Spawn an intermediate function graph for this compilation using the specified desired name (which may be modified to make it unique */
	UEdGraph* SpawnIntermediateFunctionGraph(const FString& InDesiredFunctionName);

	/* Obtains the list of compile time generated function graphs. */
	TArray<UEdGraph*>& GetGeneratedFunctionGraphs() const { return CompilerContext->GeneratedFunctionGraphs; }

	/* Find the generated FProperty that represents the given GraphNode. */
	FProperty* GetAllocatedPropertyFromNode(UPaperZDAnimGraphNode_Base* GraphNode) const;

	/* Gets the message log associated to the compiler. */
	FCompilerResultsLog& GetMessageLog() const;

	/* Adds an AnimDataLinkRecord. */
	void AddAnimDataLinkRecord(const FPaperZDAnimDataLinkRecord& Record);

	/* Obtains the consolidated event graph. */
	UEdGraph* GetConsolidatedEventGraph() const;

	/* Obtains the AnimBlueprint being compiled. */
	const UPaperZDAnimBP* GetAnimBP() const;

	/* Processes all the supplied animation nodes. */
	void ProcessAnimationNodes(TArray<UPaperZDAnimGraphNode_Base*>& AnimNodeList);

	/* Prunes any nodes that aren't reachable via an AnimData link. */
	void PruneIsolatedAnimationNodes(const TArray<UPaperZDAnimGraphNode_Base*>& RootSet, TArray<UPaperZDAnimGraphNode_Base*>& GraphNodes);

	/* Performs standard validation on the graph (outputs point to inputs, no more than one connection to each input, types match on both ends, etc...). */
	bool ValidateGraphIsWellFormed(UEdGraph* InGraph) const;

	/* Returns the allocation index of the specified node, processing it if it was pending. */
	int32 GetAllocationIndexOfNode(UPaperZDAnimGraphNode_Base* VisualAnimNode) const;

	/* Gets all anim graph nodes that are piped into the provided node (traverses input pins). */
	void GetLinkedAnimNodes(UPaperZDAnimGraphNode_Base* InGraphNode, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const;

	/* Obtains the map that relates the source nodes and the processed nodes. */
	TMap<UPaperZDAnimGraphNode_Base*, UPaperZDAnimGraphNode_Base*>& GetSourceNodeToProcessedNodeMap() const;

	/* Delegate called when starting compiling a class. */
	FOnCompileClassSignature& OnStartCompilingClass();

	/* Delegate called when finishing compiling a class. */
	FOnCompileClassSignature& OnFinishCompilingClass();

	/* Delegate called when PostExpansionStep is called. */
	FOnPostExpansionStepSignature& OnPostExpansionStep();

	/* Delegate called when copying the default data to the CDO. */
	FOnCopyTermDefaultsSignature& OnCopyTermDefaultsToDefaultObject();	

	/* Delegate called just before the animation nodes are processed. */
	FOnProcessAnimationNodesSignature& OnPreProcessAnimationNodes();

	/* Delegate called after the animation nodes have been processed. */
	FOnProcessAnimationNodesSignature& OnPostProcessAnimationNodes();
};
