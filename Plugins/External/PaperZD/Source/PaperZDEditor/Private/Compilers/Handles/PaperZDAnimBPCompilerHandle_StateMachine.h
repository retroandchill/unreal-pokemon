// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Compilers/Handles/IPaperZDAnimBPCompilerHandle.h"

class FPaperZDAnimBPGeneratedClassAccess;
class UPaperZDAnimGraphNode_Base;
class UPaperZDTransitionGraphNode_Result;
class UPaperZDK2Node_AnimGetter;
class FPaperZDAnimBPCompilerAccess;
class FPaperZDAnimBPGeneratedClassAccess;
struct FPaperZDAnimStateMachineTransitionRule;
class UEdGraph;


/**
 * Handle that processes the sub-graphs that the StateMachine nodes contain inside.
 * Replicates the "MergeUbergraphPagesIn" process of the AnimationGraph compilation part and adds the nodes from the internal graph into the
 * consolidated event graph.
 */
class FPaperZDAnimBPCompilerHandle_StateMachine : public IPaperZDAnimBPCompilerHandle, public TUniqueClassIdentifier<FPaperZDAnimBPCompilerHandle_StateMachine>
{
	/* List of AnimGetters that were found on the state machine. */
	TArray<UPaperZDK2Node_AnimGetter*> AnimGetterNodes;

public:
	//~Begin IPaperZDAnimBPCompilerHandle Interface
	virtual void Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess) override;
	//~End IPaperZDAnimBPCompilerHandle Interface

	/* Processes the given graph and any Animation node contained inside, returning the index of the SourceRootNode. */
	int32 ProcessAnimationGraph(UEdGraph* SourceGraph, UPaperZDAnimGraphNode_Base* SourceRootNode, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

	/* Processes the given Transition graph, creating a transition rule from it. */
	void ProcessTransitionGraph(UEdGraph* SourceGraph, FPaperZDAnimStateMachineTransitionRule& OutTransitionRule, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

	/* Called when starting the compilation of the class. */
	void HandleStartCompilingClass(const UClass* InClass, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

	/* Applies a final processing pass to build the AnimGetter and every other node that would require the state machine to be completely processed first. */
	void PostProcessAnimationNodes(TArrayView<UPaperZDAnimGraphNode_Base*> InAnimNodes, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

private:
	/* Generates a valid transition function name that doesn't collide with any kismet name nor any generated function name. */
	FName GenerateValidTransitionFunctionName(FPaperZDAnimBPCompilerAccess& InCompilationContext, const FString& InBaseName) const;

	/* Wires an AnimGetter node. */
	void AutoWireAnimGetter(UPaperZDK2Node_AnimGetter* AnimGetter, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);
};

//Factory definition
typedef TPaperZDCompilerHandleFactory<FPaperZDAnimBPCompilerHandle_StateMachine> FPaperZDCompilerFactory_StateMachine;
