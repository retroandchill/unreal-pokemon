// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "PaperZDAnimTransitionGraph.generated.h"

class UPaperZDTransitionGraphNode_Result;

/**
 * A graph used for evaluating the State machine transition rules.
 */
UCLASS()
class UPaperZDAnimTransitionGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:	
	/* Obtain the exit point of the graph. */
	UPaperZDTransitionGraphNode_Result* GetResultNode() const;
};
