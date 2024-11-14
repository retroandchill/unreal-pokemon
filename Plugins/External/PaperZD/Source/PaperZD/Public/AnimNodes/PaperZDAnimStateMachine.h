// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "PaperZDAnimStateMachine.generated.h"

/**
 * Contains the "rule" that governs if a transition/conduit can be taken or not.
 * Internally points to a runtime getter node for the baked transition graph.
 */
USTRUCT()
struct FPaperZDAnimStateMachineTransitionRule
{
	GENERATED_BODY()

	/* True if the rule contains blueprint logic and is bound to an UFunction. */
	UPROPERTY()
	bool bDynamicRule;

	/* Name of the function that represents the logic for a dynamic transition rule. */
	UPROPERTY()
	FName RuleFunctionName;

	/* Value of the transition rule, if it has no logic bound to it (constant pin value). */
	UPROPERTY()
	bool bConstantValue;

public:
	FPaperZDAnimStateMachineTransitionRule()
	: bDynamicRule(false)
	, RuleFunctionName(NAME_None)
	, bConstantValue(false)
	{}

	/* Evaluates the transition rule, returning its value. */
	bool EvaluateRule(UObject* AnimInstance) const;
};

/**
 * A link between two animation nodes on the state machine.
 * Has rules for knowing whether we should enter or not.
 */
USTRUCT()
struct FPaperZDAnimStateMachineLink
{
	GENERATED_BODY()

	/* Index of the rule that drives this transition. */
	UPROPERTY()
	int32 TransitionRuleIndex;

	/* Index of the target state. */
	UPROPERTY()
	int32 TargetNodeIndex;

	/* Index of the SinkNode for the "Transitional AnimGraph". */
	UPROPERTY()
	int32 TransitionalAnimNodeIndex;

public:
	//ctor
	FPaperZDAnimStateMachineLink()
	: TransitionRuleIndex(INDEX_NONE)
	, TargetNodeIndex(INDEX_NONE)
	, TransitionalAnimNodeIndex(INDEX_NONE)
	{}

	//True is there's a transitional animation that should be played when passing through this link
	FORCEINLINE bool HasTransitionalAnimations() const { return TransitionalAnimNodeIndex != INDEX_NONE; }
};

/**
 * Defines a "node" in the state machine, which can either be an AnimationState or a Conduit.
 * Nodes are defined by being connected by transitions/links.
 */
USTRUCT()
struct FPaperZDAnimStateMachineNode
{
	GENERATED_BODY()

	/* Index of the SinkNode that represents the internal AnimGraph on the generated class. */
	UPROPERTY()
	int32 AnimNodeIndex;

	/* Array of outward transitions, ordered by priority. */
	UPROPERTY()
	TArray<FPaperZDAnimStateMachineLink> OutwardLinks;

	/* If true, this node represents a conduit and not a final state. */
	UPROPERTY()
	bool bConduit;

	/* Index of the transition rule that governs the node, if it's a conduit. */
	UPROPERTY()
	int32 ConduitRuleIndex;

	/* Optional name of a custom event to call when the state machine enters this node. */
	UPROPERTY()
	FName OnStateEnterEventName;

	/* Optional name of a custom event to call when the state machine exits this node. */
	UPROPERTY()
	FName OnStateExitEventName;

public:
	//ctor
	FPaperZDAnimStateMachineNode()
	: AnimNodeIndex(INDEX_NONE)
	, bConduit(false)
	, ConduitRuleIndex(INDEX_NONE)
	{}
};

/**
 * Holds the structure of an animation State Machine for PaperZD
 */
USTRUCT()
struct FPaperZDAnimStateMachine
{
	GENERATED_BODY()

	/* Name of the state machine, mainly for debugging purposes. */
	UPROPERTY()
	FName MachineName;

	/* The index of the initial state of the machine. */
	UPROPERTY()
	int32 InitialState;

	/* The nodes that exist on the machine. */
	UPROPERTY()
	TArray<FPaperZDAnimStateMachineNode> Nodes;

	/* The transition rules (baked from the transition graphs) that we use to determine if a given link/conduit should be taken. */
	UPROPERTY()
	TArray<FPaperZDAnimStateMachineTransitionRule> TransitionRules;

	/* Jump links that can be taken as shortcuts. */
	UPROPERTY()
	TMap<FName, int32> JumpLinks;

public:
	//ctor
	FPaperZDAnimStateMachine()
	: MachineName(NAME_None)
	, InitialState(INDEX_NONE)
	{}
};
