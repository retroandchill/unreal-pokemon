// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "PaperZDStateMachineGraph.generated.h"

class UPaperZDAnimGraphNode_StateMachine;

/**
 * A State machine graph for PaperZD
 */
UCLASS()
class UPaperZDStateMachineGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	/* The owning AnimGraphNode that created this state machine graph. */
	UPROPERTY()
	TObjectPtr<UPaperZDAnimGraphNode_StateMachine> OwnerAnimGraphNode;

public:
	UPaperZDStateMachineGraph();
};
